import os
def main():
	vn = open('vertex_N.txt', 'r').readlines();
	vt = open('vertex_T.txt', 'r').readlines();
	components_changed = {}
	components_intact = {}

	for v1,v2 in zip(vn[1:],vt[1:]):
		n_vertex_info = v1.split()
		t_vertex_info =  v2.split()
		#print n_vertex_info[0]
		if n_vertex_info[1] ==  t_vertex_info[1]:
			if not components_intact.has_key( int(n_vertex_info[2])/2):
					components_intact[int(n_vertex_info[2])/2] = []
			components_intact[int(n_vertex_info[2])/2].append(n_vertex_info[0])
		else:
			if not components_changed.has_key( int(n_vertex_info[2])/2):
				components_changed[int(n_vertex_info[2])/2] = []
		
			components_changed[int(n_vertex_info[2])/2].append(n_vertex_info[0])
	
	for i in components_changed.keys():
		if len(components_changed[i]) > len( components_intact[i]):
			for vertex in components_intact[i]:
				print vertex
		else:
		  	for vertex in components_changed[i]:
		  		print vertex
if __name__ == "__main__":
	main()