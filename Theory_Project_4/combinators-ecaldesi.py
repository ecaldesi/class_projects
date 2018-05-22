#!/usr/bin/env python2.7 

import sys

if (len(sys.argv) == 2):
	# pass in file of inputs
	with open(sys.argv[1], "r") as combinator_file:
		for line in combinator_file:
			answer = []
			if line[0] == "-":
				combinator_file.close()
				break
			else:
				expression = list(line.strip()) # create expression list	
				flag = True
				visited = False # flag for left most letter
				index = 0 # keeps track of index of expression 
				new_answer = [] # second list to use for computations
				# iterate through expression
				while (index < len(expression)):
					print "=> {}".format(expression)
					# remove beginning and end parentheses if present
					if expression[0] == "(" and expression[len(expression)-1] == ")":
						del expression[0]
						del expression[len(expression)-1]
					flag = False
					temp = []
					####### S combinator #######
					if index < len(expression) and visited == False and expression[index] == "S":
						#print "in s"
						visited = True
						single = True
						flag = True
						temp = []
						little = []
						loop = 0
						index = index + 1
						extra = 0 # extra parentheses
						while (loop < 3):
							# simple expression
							if expression[index] != "(":
								temp.append(expression[index])
								index = index +1
							# complicated expression with parenthesis
							elif expression[index] == "(":
								little.append("(")
								for i in range(index+1, len(expression)):
									if expression[i] != ")":
										if expression[i] == "(":
											extra += 1
										little.append(expression[i])
									else:
										if extra == 0:
											little.append(")")
											index = i+1
											temp.append(''.join(little))
											little = [] 
											break
										else:
											extra -= 1
											little.append(")")
							loop += 1

						# create combination
						answer.append(temp[0])
						answer.append(temp[2])
						answer.append("(")
						answer.append(temp[1])
						answer.append(temp[2])
						answer.append(")")
						temp = []

						# append rest of expression
						if index < len(expression):
							if expression[index] != ")":
								answer.append(expression[index])
							index = index+1
							if (index < len(expression)):
								for o in range(index, len(expression)):
									answer.append(expression[o])
								index = o

					####### I combinator #######
					if index < len(expression) and visited == False and expression[index] == "I":
						flag = True
						single = True
						little = []
						visited = True
						index += 1
						extra = 0 # extra parenthesis
						# simple expression
						if expression[index] != "(":
							answer.append(expression[index])
							index = index +1
						# expression with parentheses
						elif expression[index] == "(":
							little.append("(")
							for i in range(index+1, len(expression)):
								if expression[i] != ")":
									if expression[i] == "(":
										extra += 1
									little.append(expression[i])
								else:
									if extra == 0:
										little.append(")")
										index = i+1
										answer.append(''.join(little))
										little = [] 
										break
									else:
										extra -= 1
										little.append(")")
						# append rest of expression
						if (index < len(expression)):
							for o in range(index, len(expression)):
								if o == index and expression[o] == ")":
									continue
								else:
									answer.append(expression[o])
							index = o

					####### K combinator #######
					if index < len(expression) and visited == False and expression[index] == "K":
						flag = True
						temp = []
						little = []
						single = True
						loop = 0
						index = index +1
						visited = True
						extra = 0
						while (loop < 2):
							old_index = index
							# simple expression
							if expression[index] not in ["(", ")"]:
								temp.append(expression[index])
								index = index +1
							# more complex
							elif expression[index] == "(":
								for i in range(index+1, len(expression)):
									if expression[i] != ")":
										little.append(expression[i])
									elif expression[i] == ")":
										l = i+1
										for l in range (i+1, len(expression)):
											if expression[l] != ")":
												break
										index = l
										temp.append(''.join(little))
										little = [] 
										break
							if old_index == index:
								index += 1
								loop -= 1
							loop += 1
						answer.append(temp[0])
						# append rest of expression
						if (index < len(expression)):
							for o in range(index, len(expression)):
								answer.append(expression[o])
							index = o


					####### SINGLE EXPRESSION NOT IN PARENTHESES e.g. b #######				
					if visited == False and single == True and index < len(expression) and expression[index] not in ["(", ")", "S", "K", "I"]:
						flag = True
						#print "here in sinlge "
						for number in range(index, len(expression)):
							if expression[number] not in ["(", ")", "S", "K", "I"]:
								answer.append(expression[number])
							else:
								break
							
						index = number

					# if S, K, I not here
					if flag == False:
						if expression[index] in ["(", ")"] and index != 0:
							answer.append(expression[index])
						index += 1


					seen = 0 # keep track of matching parenthesis

					# RESET INDEX AND EXPRESSION AND ITERATE THROUGH LOOP AGAIN
					if (index >= len(expression)):
						visited = False
						single = False
						string = ''.join(answer)
						new_answer = list(string)
						answer = new_answer
						if "I" in answer:
							expression = []
							# eliminate empty parentheses
							for r in range(0, len(answer)):
								if answer[r] == "(":
									if r+1 < len(answer):
										if answer[r+1] == ")":
											del answer[r]
											del answer[r+1]
											break
									
							index = 0
							single = True
							visited = False
							expression = answer
							answer = []	
						if "K" in answer:
							expression = []
							# eliminate empty parentheses
							for r in range(0, len(answer)) :
								if answer[r] == "(":
									f = True
									if r+1 < len(answer):
										if answer[r+1] == ")":
											del answer[r]
											del answer[r+1]
											break
									
							index = 0
							visited = False
							single = True
							expression = answer
							answer = []
						if "S" in answer:
							expression = []
							# eliminate empty parentheses
							for r in range(0, len(answer)):
								if answer[r] == "(":
									if r+1 < len(answer):
										if answer[r+1] == ")":
											del answer[r]
											del answer[r+1]
											break
							
							index = 0
							visited = False
							single = True
							expression = answer
							answer = []
							
				seen = 0
				parenthesis = []

				# DELETE PARENTHESIS THAT AREN'T MATCHING
				for t in range(0, len(answer)):
					if answer[t] == "(":
						seen += 1
						parenthesis.append("(")
					if answer[t] == ")":
						parenthesis.append(")")
						seen -= 1
				counter = 0
				# if there's only one parenthesis
				if len(parenthesis) == 1:
					for il in range(0, len(answer)) :
						if answer[il] in ["(", ")"]:
							del answer[il]
							seen = 0
							break
				# if there is two parenthesis facing opposite ways
				if len(parenthesis) == 2 and parenthesis == [")", "("]:
					for item in answer[:]:
						if item in ["(", ")"]:
							answer.remove(item)
					seen = 0	
				evict = 0
				# if there is more (
				if seen > 0:
					for op in range(0, len(parenthesis)):
						if op+1 < len(parenthesis) and parenthesis[op] == "(" and parenthesis[op+1] == "(":
							evict = op
					if evict != 0:	
						for il in range(0, len(answer)) :
							if answer[il] == "(" and counter < evict :
								counter += 1
							if counter == evict:
								del answer[il]
								break
				evict = 0
				# if there is more )
				if seen < 0:
					for op in range(0, len(parenthesis)):
						if op+1 < len(parenthesis) and parenthesis[op] == ")" and parenthesis[op+1] == ")":
							evict = op
					if evict != 0:		
						for il in range(0, len(answer)) :
							if answer[il] == ")" and counter < evict :
								counter += 1
							if counter == evict:
								del answer[il]
								break
				counter = 0
				seen = 0	
				print ''.join(answer)
