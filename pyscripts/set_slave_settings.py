import os
import re
import inspect
from argparse import ArgumentParser
from subprocess import call

def construct_slave_settings(CAN_id, periods, Kp, Ki, Kd):
    return {"CAN_id":  CAN_id,
            "periods": periods,
            "PID":     {"Kp": Kp, "Ki": Ki, "Kd": Kd}}
				
# Set the dictionary with settings for each slave
settings = {"Entrance": construct_slave_settings(CAN_id=801, 
                                                 periods=[260, 248, 251, 251], 
												 Kp=[2e-2, 2e-2, 2e-2, 2e-2], 
												 Ki=[1.45e-5, 1.45e-5, 1.45e-5, 1.45e-5], 
												 Kd=[1e-1, 1e-1, 1e-1, 1e-1]),
			"Exit2": construct_slave_settings(CAN_id=803, 
                                                 periods=[260, 260, 260, 260], 
												 Kp=[0, 0, 0, 0], 
												 Ki=[0, 0, 0, 0], 
												 Kd=[0, 0, 0, 0]),
			"Exit1": construct_slave_settings(CAN_id=802, 
                                                 periods=[256, 262, 251, 251], 
												 Kp=[2e-2, 2e-2, 2e-2, 2e-2], 
												 Ki=[1.45e-5, 1.45e-5, 1.45e-5, 1.45e-5], 
												 Kd=[1e-1, 1e-1, 1e-1, 1e-1]),
           }
		   
current_path = os.path.dirname(os.path.abspath(inspect.getfile(inspect.currentframe())))

parser = ArgumentParser()
parser.add_argument("SlaveID", help="ID of slave you want to program. Can be either Entrance, Exit1 or Exit2")
args = parser.parse_args()

slave_id = args.SlaveID
if slave_id in settings:
    CAN_id = settings[slave_id]['CAN_id']
    periods = settings[slave_id]['periods']
    Kp = settings[slave_id]['PID']['Kp']
    Ki = settings[slave_id]['PID']['Ki']
    Kd = settings[slave_id]['PID']['Kd']
else:
    raise Exception("Wrong SlaveID provided.")

#############################################################
# Modify the CAN id
#############################################################
interrupt_filename = "%s/../src/stm32f4xx_it.c" % current_path
temp_interrupt_filename = "%s.temp" % interrupt_filename
call(['cp', interrupt_filename, temp_interrupt_filename])

with open(interrupt_filename, 'r') as interrupt_file:
    content = interrupt_file.read()

CAN_id_pattern_base = "const uint32_t thisSlaveCANstdID = "
CAN_id_pattern = r"%s\d*;" % CAN_id_pattern_base
subs = "%s%d;" % (CAN_id_pattern_base, CAN_id)
content = re.sub(CAN_id_pattern, subs, content)
with open(temp_interrupt_filename, 'w') as tmp_file:
    tmp_file.write(content)

# replace the file with rewritten content
call(['rm', interrupt_filename])
call(['mv', temp_interrupt_filename, interrupt_filename])

#############################################################
# Modify the resonance frequencies
#############################################################
resfreq_filename = "%s/../src/phil_pwm.c" % current_path
temp_resfreq_filename = "%s.temp" % resfreq_filename
call(['cp', resfreq_filename, temp_resfreq_filename])

with open(resfreq_filename, 'r') as resfreq_file:
    content = resfreq_file.read()

periods_pattern_base = "static uint16_t PWM_PERIODS[N_MOTORS] = {"
periods_pattern = r"static uint16_t PWM_PERIODS\[N_MOTORS\] = \{.*;"
subs = "%s%s};" % (periods_pattern_base, ', '.join(map(str, periods)))

content = re.sub(periods_pattern, subs, content)
with open(temp_resfreq_filename, 'w') as tmp_file:
    tmp_file.write(content)

# replace the file with rewritten content
call(['rm', resfreq_filename])
call(['mv', temp_resfreq_filename, resfreq_filename])

#############################################################
# Modify the PID coefficients
#############################################################
pid_filename = "%s/../src/phil_pwm.c" % current_path
temp_pid_filename = "%s.temp" % pid_filename
call(['cp', pid_filename, temp_pid_filename])

with open(pid_filename, 'r') as pid_file:
    content = pid_file.read()

pid_pattern_base = "static double PIDSettings[N_MOTORS][3] =  {"
pid_pattern = r"static double PIDSettings\[N_MOTORS\]\[3\] =  \{.*};"

substitution_dict = {'pattern': pid_pattern_base,
                     'motor1': ', '.join(map(str,[Kp[0], Ki[0], Kd[0]])),
                     'motor2': ', '.join(map(str,[Kp[1], Ki[1], Kd[1]])),
					 'motor3': ', '.join(map(str,[Kp[2], Ki[2], Kd[2]])),
					 'motor4': ', '.join(map(str,[Kp[3], Ki[3], Kd[3]]))}
subs = """%(pattern)s\n
{%(motor1)s},
{%(motor2)s},
{%(motor3)s},
{%(motor4)s}
};""" % substitution_dict

subs = 'x'

content = re.sub(pid_pattern, subs, content, flags=re.DOTALL | re.MULTILINE)
with open(temp_pid_filename, 'w') as tmp_file:
    tmp_file.write(content)

# replace the file with rewritten content
call(['rm', pid_filename])
call(['mv', temp_pid_filename, pid_filename])


