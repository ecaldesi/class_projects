# Project 2, nfa2dfa-international.py
# Elisabetta Caldesi, Jorge Nazario

import sys
import copy
from itertools import chain, combinations 

# get power set
def get_powerset(iterable):
    s = list(iterable)
    return chain.from_iterable(combinations(s, r) for r in range(1, len(s)+1))

# print output
def print_output(output_file, header, alphabet1, new_states_string, final_start_state, accept_final_string):
	output_file.write("{}\n".format(header))
	output_file.write("{}\n".format(alphabet1))
	output_file.write("{}\n".format(new_states_string))
	output_file.write("{}\n".format(final_start_state))
	output_file.write("{}\n".format(accept_final_string))

# print output for each path
def output2(output_file, path):
	output_file.write("{}\n".format(path))

# MAIN
if len(sys.argv) == 2:
	# set variables
	count = 0 # count lines read in 
	alphabet = [] # alphabet list
	states = [] # states of the FA
	states_visited = [] # states visited in the transitions
	accept = [] # accept states
	dict_states = {} # dictionary
	values = [] 
	each_path = [] # each path, to be added to dictionary
	alternate_alphabet = [] # alphabet without epsilon
	keys_seen = set()
	output_name = sys.argv[1].split(".txt")
	output_string = ''.join(output_name) + "-output.csv"
	output_file = open(output_string, 'w+')	# write to output file
	flag = 0 # check if q was stripped
	
	########## OPEN RULES FILE ##########
	with open(sys.argv[1], "r") as rules_file:
		# parse in lines to check variables
		for line in rules_file:
			# strip line
			line = line.strip()
			if count == 0:
				header = line

			if count == 1:
				alphabet1 = line
				alphabet = line.split(",")
				alternate_alphabet = alphabet[:]
				alphabet.append("~")

			if count == 2:
				states1 = line.split(",")
				for element in states1:
					if element[0] == "q":
						flag = 1
						states.append(element[1:])
					else:
						states.append(element)
			if count == 3:
				start_state = line
				if start_state[0] == "q":
					start_state = start_state[1:]
			if count == 4:
				accept1 = line.split(",")
				for element in accept1:
					if element[0] == "q":
						accept.append(element[1:])
					else:
						accept = accept1[:]
				lines = 0

			if count > 4:
				linetransition = line.split(",")
				# create dictionary for each state 
				if linetransition[0][0] == "q" and linetransition[2][0] == "q":
					linetransition[0] = linetransition[0][1:]
					linetransition[2] = linetransition[2][1:]
				if ((linetransition[1] in alphabet) and (linetransition[0] in states) and (linetransition[2] in states)):
					# if it's not the first line read, and for that key you have already seen the same input
					if (lines > 0 and linetransition[1] == each_path[0]):
						# add state to each_path 
						each_path.append(linetransition[2])
						continue
					else:
						# empty list for next input possible
						each_path = []

					# clear the values list if you have a new key
					if (linetransition[0] not in keys_seen):
						values = []

					# add input and state to each possible path
					each_path.append(linetransition[1])
					each_path.append(linetransition[2])
					# add path to values for that key
					values.append(each_path)
					# add value for key to dictionary
					dict_states[linetransition[0]] = values
					lines = lines + 1 # keep track of line
					keys_seen.add(linetransition[0]) # keep track of keys

			count = count + 1

	########## keep account of epsilon paths ##########
	epsilon = {}
	new_values = []
	for key, value in dict_states.items():
		for each_value in value:
			# if from any state there is a path with epsilon
			if each_value[0] == "~":
				new_values = each_value[1:]
				# fill in epsilon dictionary, with key as state that has an epsilon path, and value as the state where
				# epsilon takes you
				for element in new_values:
					epsilon[key] = element
		# clear new values
		new_values = []

	# update original dictionary based on epsilon paths
	for key, value in dict_states.items():
		for each_value in value:
			for element in each_value:
				for key1, value1, in epsilon.items():
					if element == key1:
						each_value.append(value1)

	copy_of_dict = {}
	copy_of_dict = copy.deepcopy(dict_states) # copy of original without trap states, without powerset
	
	########## ADD ACCEPT STATES IF THERE WASN'T A PATH FROM THEM ##########
	# check if accept states are in keys
	aflag = 0
	nval1 = []
	nval2 = []
	for each_accept in accept:
		for key, value in dict_states.items():
			if each_accept == key:
				aflag = 1
		# if accept state wasn't in the keys
		if aflag == 0:
			for let in alternate_alphabet:
				nval1.append(let)
				nval1.append('phi')
				nval2.append(nval1)
				nval1 = []	
			dict_states[each_accept] = nval2
			nval2 = []


	########## ADD TRAP STATES ##########
	my_bool = 0
	for key, value in dict_states.items():	
		for element in alternate_alphabet:
			for counter_list in range(len(value)):
				if (str(element) == str(dict_states[key][counter_list][0])):
					my_bool = 1
			if my_bool == 0:
				dict_states[key].append([element, 'phi'])
			my_bool = 0		

	# get power set of states
	new_states = []
	powerset = list(get_powerset(states))
	new_powerset = []
	for element in powerset:
		element = ''.join(element)
		new_states.append(element) # fill in list with all of the states
		if element not in states:
			new_powerset.append(element)

	new_states.append("phi") # append phi to all of the states
	new_states_string = ','.join(new_states) # print all of the states to output

	values = []
	input_seen = set()
	# fill in dictionary with new paths from power set
	for element in new_powerset:
		for char in element: 
			for key, value in copy_of_dict.items():
				if char == key:
					for list_count in value:
						if list_count[0] in alternate_alphabet:
							values.append(list_count)
							
		dict_states[element] = values
		values = []
	

	########## CHECK THAT THERE ARENT MULTIPLE LISTS WITH SAME INPUT ex: {1: [a, 2], [b, 3], [a, 4]} = {1: [a, 2, 4], [b, 3]} ##########
	new_value = []
	temp = []
	temp1 = set()
	saved = []
	for key, value in dict_states.items():
		for alpha in alternate_alphabet:
			c = 0
			for curr_list in value:
				if alpha == curr_list[0]:
					c = c + 1 # count how many identical inputs have been seen
					saved = curr_list[:]
			if c > 1: # condense
				temp = [alpha]
				for curr_list in value:
					if alpha == curr_list[0]:
						# CONDENSE [a, 1, 2, 2] = [a, 1, 2]
						for element in curr_list[1:]:
							element = int(element)
							temp1.add(element)

			elif c == 1: # only one input
				if saved[1] == 'phi':
					new_value.append(saved)
				else:
					# condense
					temp.append(saved[0])
					for n in saved[1:]:
						n = int(n)
						temp1.add(n)
			if temp:
				for t in temp1:
					temp.append(t)
				new_value.append(temp)
				temp = []

			# space here
			temp1 = set()
			saved = []
		dict_states[key] = new_value # update the dictionary
		new_value = []

	########## ADD TRAP STATE AND TRANSITIONS TO ITSELF ##########
	l1 = []
	valuephi = []
	for alpha in alternate_alphabet:
		l1.append(alpha)
		l1.append('phi')
		valuephi.append(l1)
		l1 = []

	dict_states['phi'] = valuephi 
	

	########## CONVERT EVERY VALUE TO A STRING ##########
	new1 = []
	new2 = []
	for key, value in dict_states.items():
		for curr_list in value:
			for i in curr_list:
				i = str(i)
				new1.append(i)
			
			new2.append(new1)
			new1 = []
		dict_states[key] = new2
		new2 = []

	accept_final = []

	########## FINALIZE ACCEPT STATE ##########
	for key, value in dict_states.items():
		for char in key:
			if char in accept:
				accept_final.append(key)

	accept_final_string = ','.join(accept_final)	

	
	########## FINALIZE START STATE ##########
	start_state_list = []
	start_state_list.append(start_state)

	########### CHECK IF START HAS AN EPSILON PATH ##########
	for curr_list in copy_of_dict[start_state]:
		if curr_list[0] == "~":
			a = ''.join(curr_list[1:])
			start_state_list.append(a)
			
	final_start_state = ''.join(start_state_list)

	########## GO THROUGH DICTIONARY, FILL IN WITH PHIS FOR EVERY MISSING PATH FOR LETTER ##########
	check_alpha = []
	ll = []
	cin = 0
	for key, value in dict_states.items():
		for l in value:
			check_alpha.append(l[0])
		for inp in check_alpha:
			if inp in alternate_alphabet:
				cin = cin + 1
		if cin != len(alternate_alphabet):
			for lett in alternate_alphabet:
				if lett not in check_alpha:
					ll.append(lett)
					ll.append('phi')
					value.append(ll)
		ll = []	
		cin = 0
		check_alpha = []
			
	########## REAPPEND Q IF STRIPPED ##########
	new_states_list1 = []
	accept_final1 = []
	if flag == 1:
		# append q to each state before saving them into a string of states
		for st in new_states: # new_state is the list containing all of the states
			if st != "phi":
				st = "q"+st
			new_states_list1.append(st)
		new_states_string1 = ",".join(new_states_list1) # put all of the states in a string

		# add q to start state
		final_start_state1 = "q" + final_start_state

		# add q to final states
		for each_accept in accept_final:
			accept_final1.append("q"+each_accept)
			accept_final_string1 = ','.join(accept_final1)		
		# PRINT OUTPUT
		print_output(output_file, header, alphabet1, new_states_string1, final_start_state1, accept_final_string1)
	else:			
		print_output(output_file, header, alphabet1, new_states_string, final_start_state, accept_final_string)


	########## COMPOSE PATH ##########
	value = 0
	for key, value in dict_states.items():
		for curr_list in value:
			if curr_list[0] != "~":	
				string = ''.join(curr_list[1:])
				#print string
				if key != "phi" and flag == 1:
					new_key = "q"+key
					if string != "phi" and flag == 1:
						new_string = "q"+string
						if (value != 2 or value == 0):
							path = new_key + "," + curr_list[0] + "," + new_string
							value = 1
					if (value != 1 or value == 0):
						path = new_key + "," + curr_list[0] + "," + string
						value = 2
				if (value != 1 and value != 2):
					path = key + "," + curr_list[0] + "," + string
				output2(output_file, path)
				value = 0

	output_file.close()
