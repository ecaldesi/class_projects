# Elisabetta Caldesi
# Turing Machine - 1 tape

# libraries
import sys

# MAIN
if len(sys.argv) == 3:
	# set variables
	count = 0 # count lines read in 
	input_alphabet = []
	tape_alphabet = []
	name_list = [] # temporary list to read first line
	states = [] # list of states
	k = 0 # number of tapes
	keys_seen = set()
	each_path = [] # each path, to be added to dictionary
	values = [] # list that contains all of the possible paths from one state	
	dict_states = {}

	states_visited = [] # states visited for the string given
	rules_list = [] # keep track of rules
	line_list = [] # used to split the line for the rules
	rules_counter = 1

	# print tape name to stdout
	print sys.argv[2]


	########## OPEN RULES FILE ##########
	with open(sys.argv[1], "r") as rules_file:
		# parse in lines to check variables
		for line in rules_file:
			# strip line
			line = line.strip()
			if count == 0:
				name_list = line.split(",")
				k = int(name_list[1])
				if k > 1:
					print "Error, the machine can only support 1 tape!"
					sys.exit(1)
				tape_length = name_list[2]
				steps = name_list[3]
			if count == 1:
				input_alphabet = line.split(",")
				if (("*" in input_alphabet) or ("_" in input_alphabet)):
					print "Rejected"
					sys.exit(0)
			if count == 2:
				states = line.split(",")
			if count == 3:
				start_state = line
			if count == 4:
				line = line.split(",")
				accept_state = line[0]
				reject_state = line[1]
				lines = 0	
			if count == 5:
				tape_alphabet = line.split(",")
				if (("*" in tape_alphabet) or ("_" in tape_alphabet)):
					print "Rejected"
					sys.exit(0)
				tape_alphabet.append("_")	
			if count > 5:
				print "Rule {}: {}".format(rules_counter, line)
				line_list = line.split(',')
				rules_list.append(line_list)
				linetransition = line.split(",")
				if ((linetransition[1] in ["_","*"] or linetransition[1] in tape_alphabet) \
				and (linetransition[0] in states) and (linetransition[2] in states) \
				and (linetransition[3] in ["_","*"] or linetransition[3] in tape_alphabet) \
				and (linetransition[4] in ["L", "R", "S"])):
					each_path = []
					# clear the values list if you have a new key
					if (linetransition[0] not in keys_seen):
						values = []

					# add input and state to each possible path
					each_path.append(linetransition[1])
					each_path.append(linetransition[2])
					each_path.append(linetransition[3])
					each_path.append(linetransition[4])

					# add path to values for that key
					values.append(each_path)
					# add value for key to dictionary
					dict_states[linetransition[0]] = values
					lines = lines + 1 # keep track of line
					keys_seen.add(linetransition[0]) # keep track of keys

				rules_counter = rules_counter + 1
			count = count + 1;

	########## OPEN TAPE FILE ##########
	with open(sys.argv[2], "r") as tape_file:
		print "\n"
		num = 0 # counter for rule number
		for line in tape_file:
			index = 0
			line = line.strip()
			# check if length of tape is over max length allowed
			if len(line) -1 > tape_length:
				print "Reject, tape too long!"
				break
			# initialize tape
			tape = list(line)
			# initialize variables
			states_visited.append(start_state)
			current_state = start_state
			print "Tape i: {}".format(line[:-1])
			counter_steps = 0
			input_val = tape[index]
			# flag to exit while loop
			flag = True
			while (flag):
				num = 0 # counter for rules
				if (input_val in tape_alphabet or input_val in ["_","*"]):
					input_valid = 1
					path_found = 0
					for key, value in dict_states.items(): 
						if (current_state == key) and (path_found == 0): #find key
							for path in value: # each list in value
								if (path[0] == input_val) or (input_val == "*"): # if the input of the list is equal to the input give
									path_found = 1
									if counter_steps < steps:
										counter_steps = counter_steps + 1
									else:
										flag = False
										print "Too many steps!"
										break
									prev_state = current_state # set the previous state to current state
									current_state = path[1] # set new current state
									states_visited.append(current_state) # append to list of states visited	
									#don't change character under tape if input is *
									if (path[2] != "*"):
										tape[index] = path[2]
									if flag == True:	
										if path[3] == "R":
											if index < len(tape)-1:
												index = index + 1
											input_val = tape[index]
											for i in rules_list: # find rule number
												num = num + 1
												if i[0] == key: # check if first state of the rule is equal to key
													if path[0] == i[1]: # found a match, input matches input in the rule
														break
											print "{},{},{},{},{},{},{},{}".format(counter_steps, num, index-1, \
											key,path[0], path[1], path[2], path[3]) # OUTPUT
											break
										elif path[3] == "L":
											if index > 0:
												index = index - 1
											input_val = tape[index]
											for i in rules_list: # find rule number
												num = num + 1
												if i[0] == key: # check if first state of the rule is equal to key
													if path[0] == i[1]: # found a match, input matches input in the rule
														break
											print "{},{},{},{},{},{},{},{}".format(counter_steps, num, index+1, \
											key,path[0], path[1], path[2], path[3]) # OUTPUT
											break
										elif path[3] == "S":
											index = index
											input_val = tape[index]
											for i in rules_list: # find rule number
												num = num + 1
												if i[0] == key: # check if first state of the rule is equal to key
													if path[0] == i[1]: # found a match, input matches input in the rule
														break
											print "{},{},{},{},{},{},{},{}".format(counter_steps, num, index, \
											key,path[0], path[1], path[2], path[3]) # OUTPUT
											break
							if path_found == 0:
								states_visited.append(reject_state)
								flag = False
								break	
				elif (flag == True) and (input_val not in tape_alphabet):
					flag = False
				# exit while loop
				if (states_visited[-1] == accept_state or states_visited[-1] == reject_state):
					flag = False
			if input_valid == 1:
				print "Tape i: {}".format(''.join(tape[:-1]))
				if states_visited[-1] == accept_state:
					print "Accepted\n"
				elif states_visited[-1] == reject_state:
					print "Rejected\n"
				else :
					print "Error\n"
				del states_visited[:]
				del tape[:]
