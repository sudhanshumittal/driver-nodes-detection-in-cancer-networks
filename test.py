import pickle
PostingStruct = recordtype("PostingStruct", "df posting") # a struct consting of df and posting list file pointer

def main():
	p = PostingStruct 
	with open('./pickle/advanc.pik', 'rb') as f:
			p = pickle.load(f)
	print p.posting
if __name__ == '__main__':
	main()