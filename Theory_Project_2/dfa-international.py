# Project 2, dfa-international.py
# Elisabetta Caldesi, Jorge Nazario

# libraries
import sys
from collections import defaultdict

# MAIN
if len(sys.argv) == 3:
	# set variables
	count = 0 # count lines read in 
	alphabet = []
	states = []
	states_visited = [] # states visited for the string given
	accept = [] # accept states
	state_paths = defaultdict(list) # dictionary
	input_state = [] # list for value in dictionary
	rules_list = [] # keep track of rules
	line_list = [] # used to split the line for the rules
	rules_counter = 1

	# print rules file and string file names
	print "{}".format(sys.argv[1])
	print "{}".format(sys.argv[2])

	########## OPEN RULES FILE ##########
	with open(sys.argv[1], "r") as rules_file:
		# parse in lines to check variables
		for line in rules_file:
			# strip line
			line = line.strip()
			if count == 1:
				alphabet = line.split(",")
			if count == 2:
				states = line.split(",")
			if count == 3:
				start_state = line
			if count == 4:
				accept = line.split(",")	
			if count > 4:
				print "Rule {}: {}".format(rules_counter, line)
				line_list = line.split(',')
				rules_list.append(line_list)
				linetransition = line.split(",")
				if ((linetransition[1] in alphabet) and (linetransition[0] in states) and (linetransition[2] in states)):
					# fill in input_state list for single input and state destination given a source state
					input_state.append(linetransition[1])
					input_state.append(linetransition[2])
					# state_paths dictionary of paths possible from each state given input 
					state_paths[linetransition[0]].append(input_state)	
					input_state = [] # clear
				rules_counter = rules_counter + 1
			count = count + 1;

	########## OPEN STRINGS FILE ##########
	with open(sys.argv[2], "r") as strings_file:
		num = 0 # counter for rule number
		for line in strings_file:
			# initialize variables
			states_visited.append(start_state)
			current_state = start_state
			line = line.strip()
			print "\nString: {}".format(line)
			counter_steps = 0
			for input_val in line: # check for every input in the string
				num = 0
				if input_val in alphabet:
					input_valid = 1
					path_found = 0
					for key, value in state_paths.items(): 
						if (current_state == key) and (path_found == 0):
							for path in value: # each list in value 
								if path[0] == input_val: # if the input of the list is equal to the input give
									for i in rules_list: # find rule number
										num = num + 1
										if i[0] == key: # check if first state of the rule is equal to key
											if path[0] == i[1]: # found a match, input matches input in the rule
												break
									counter_steps = counter_steps + 1
									path_found = 1
									prev_state = current_state # set the previous state to current state
									current_state = path[1] # set new current state
									states_visited.append(current_state) # append to list of states visited	
									print "{},{},{},{},{}".format(counter_steps, num, prev_state, input_val, current_state) # OUTPUT
							if path_found == 0:
								print "path wasn't found"
								sys.exit(1)	
				else:
					print "input not valid"
					sys.exit(1)
			if input_valid == 1:
				if states_visited[-1] in accept:
					print "Accepted"
				else:
					print "Rejected"
				del states_visited[:]
