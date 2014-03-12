def main():
	fo = open("component_edge_list.txt",'w')
	i = 0
	j = 0 
	for fp in open("component_graph_T.txt",'r').readlines():
		j = 0
		for edge in fp.split():
			if edge == "1":
				#edge found
				fo.write(str(i)+"\t"+str(j)+"\n")
				print str(i)+"\t"+str(j)+"\n"
			j+=1
		i+=1
if __name__ == "__main__":
    main()		 