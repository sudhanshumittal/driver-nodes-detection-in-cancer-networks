#########
# USAGE #
#########

	filter_data.cpp:
		one time operation.
		eliminates all proteins from the gene expression data not in ppi file and stores the remaing in 'filtered' folder.

	pearson.cpp:
		execute this if you wish to lower the threshold value. currently its 0.5

	main.cpp:
		creates a graph with skeleton as 
		labels every edge as Xor, Nxor or Dont_Care
		traverses the graph to label all the edges.
	
################
# OBSERVATIONS #
################


number of unique nodes in ppi file is = 5814

filtering

	number of nodes in filtered gene expression file (normal case) = 5551
	number of nodes in filtered gene expression file (cancer case) = 5551

correation values
	sample size = 15
	threshold = 0.5
	number of significant correlations found (normal case)  = 2561
	number of significant correlations found (disease case)  = 2339

edge labelling 
	
	Normal case:
		number of positively correlated pairs = 218
		number of negatively correlated pairs = 159
	Tumor Case:
		number of positively correlated pairs = 294
		number of negatively correlated pairs = 233

filps
	with skeleton = 0
	without = 2736
CASE TOGGLED ONLY
number of edges retained = 135
number of edges that flipped = 60
number of edges that remained unchanged = 75
	
	Normal case:
		number of positively correlated pairs = 70
		number of negatively correlated pairs = 65
	Tumor Case:
		number of positively correlated pairs = 60
		number of negatively correlated pairs = 75


#############
## DETAILS ##
#############

filtering

	some letters in geneexpression file were lowercase, they have been made uppercase.
	'///'' were split into different proteins.

normal: AHSA1,FANCC,	 cancer: AHSA1,FANCC,
normal: HSP90AA1,	 cancer: HSP90AA1,
normal: AMFR,	 cancer: AMFR,
normal: UBE2D4,	 cancer: UBE2D4,
normal: APBB1,SF3B2,SF3B4,	 cancer: APBB1,SF3B2,
normal: 	 cancer: SF3B4,
normal: APPBP2,PCSK5,	 cancer: APPBP2,
normal: 	 cancer: PCSK5,
normal: ARAF,	 cancer: ARAF,ASS1,
normal: ASS1,	 cancer: 
normal: ARL2BP,RAC1,	 cancer: ARL2BP,RAC1,
normal: 	 cancer: 
normal: ARRB2,RPS7,	 cancer: ARRB2,
normal: 	 cancer: RPS7,
normal: ASF1B,	 cancer: ASF1B,RPL31,
normal: RPL31,	 cancer: 
normal: BCL6,ERCC5,	 cancer: BCL6,
normal: NCOR1,	 cancer: ERCC5,NCOR1,
normal: BPTF,MPHOSPH8,SUMO2,	 cancer: BPTF,
normal: 	 cancer: MPHOSPH8,SUMO2,
normal: C12ORF44,DSC1,	 cancer: C12ORF44,
normal: 	 cancer: DSC1,
normal: CAPNS1,	 cancer: CAPNS1,RAB1A,
normal: RAB1A,	 cancer: 
normal: CLIP1,	 cancer: CLIP1,
normal: PAFAH1B1,	 cancer: PAFAH1B1,
normal: CLUAP1,MAGEA11,	 cancer: CLUAP1,MAGEA11,
normal: 	 cancer: 
normal: DDX17,SH3KBP1,	 cancer: DDX17,
normal: RPA1,	 cancer: RPA1,SH3KBP1,
normal: DZIP3,UBE2D1,	 cancer: DZIP3,
normal: 	 cancer: UBE2D1,
normal: EED,ZFP42,	 cancer: EED,
normal: 	 cancer: ZFP42,
normal: EEF1A1,	 cancer: EEF1A1,
normal: PAFAH1B3,	 cancer: PAFAH1B3,
normal: EXOSC8,MKRN1,	 cancer: EXOSC8,
normal: 	 cancer: MKRN1,
normal: FAF2,MYC,UBR2,	 cancer: FAF2,
normal: 	 cancer: MYC,UBR2,
normal: FASTKD5,	 cancer: FASTKD5,
normal: IRAK2,	 cancer: IRAK2,
normal: GCN1L1,SIRT7,	 cancer: GCN1L1,SIRT7,
normal: 	 cancer: 
normal: GIPC1,	 cancer: GIPC1,PDXDC1,
normal: PDXDC1,	 cancer: 
normal: GOT2,	 cancer: GOT2,
normal: MPG,	 cancer: MPG,
normal: HGS,NF2,	 cancer: HGS,NF2,
normal: 	 cancer: 
normal: HLTF,	 cancer: HLTF,
normal: UBE2Z,	 cancer: UBE2Z,
normal: HNRNPA1,	 cancer: HNRNPA1,PTBP1,
normal: PTBP1,	 cancer: 
normal: HSPA9,TP53,	 cancer: HSPA9,TP53,
normal: 	 cancer: 
normal: IGHA1,PCBD1,RAB11B,UBC,	 cancer: IGHA1,PCBD1,
normal: SKIV2L2,UBASH3B,	 cancer: RAB11B,SKIV2L2,UBASH3B,UBC,
normal: KDM5B,	 cancer: KDM5B,
normal: SMARCC2,	 cancer: SMARCC2,
normal: KHDRBS2,RBM39,	 cancer: KHDRBS2,
normal: 	 cancer: RBM39,
normal: LDHA,	 cancer: LDHA,
normal: PCK1,	 cancer: PCK1,
normal: PIAS4,SMAD2,	 cancer: PIAS4,SMAD2,
normal: 	 cancer: 
normal: PML,	 cancer: PML,
normal: PPARG,	 cancer: PPARG,
normal: PPP2R1B,RALA,	 cancer: PPP2R1B,RALA,
normal: 	 cancer: 
normal: RANBP2,	 cancer: RANBP2,
normal: RAPGEF3,	 cancer: RAPGEF3,
normal: RFC4,RPAP2,	 cancer: RFC4,RPAP2,
normal: 	 cancer: 
normal: RNF10,	 cancer: RNF10,
normal: UBE2W,	 cancer: UBE2W,
normal: TBP,WDR62,	 cancer: TBP,WDR62,
normal: 	 cancer: 
normal: VCP,	 cancer: VCP,YOD1,
normal: YOD1,	 cancer: 
YOD1

##############################

min toggled vertices
########################

SF3B4     This gene encodes one of four subunits of the splicing factor 3B. The protein encoded by this gene cross-links to a
    region in the pre-mRNA immediately upstream of the branchpoint sequence in pre-mRNA in the prespliceosomal complex A.
    It also may be involved in the assembly of the B, C and E spliceosomal complexes. In addition to RNA-binding activity,
    this protein interacts directly and highly specifically with subunit 2 of the splicing factor 3B. This protein
    contains two N-terminal RNA-recognition motifs (RRMs), consistent with the observation that it binds directly to
    pre-mRNA. (provided by RefSeq, Jul 2008) 
PCSK5	proprotein convertase subtilisin/kexin type 5 
	    The protein encoded by this gene belongs to the subtilisin-like proprotein convertase family. The members of this
    family are proprotein convertases that process latent precursor proteins into their biologically active products. This
    encoded protein mediates posttranslational endoproteolytic processing for several integrin alpha subunits. It is
    thought to process prorenin, pro-membrane type-1 matrix metalloproteinase and HIV-1 glycoprotein gp160. Multiple
    transcript variants encoding different isoforms have been found for this gene. (provided by RefSeq, Jun 2010) 

ASS1
    The protein encoded by this gene catalyzes the penultimate step of the arginine biosynthetic pathway. There are
    approximately 10 to 14 copies of this gene including the pseudogenes scattered across the human genome, among which
    the one located on chromosome 9 appears to be the only functional gene for argininosuccinate synthetase. Mutations in
    the chromosome 9 copy of this gene cause citrullinemia. Two transcript variants encoding the same protein have been
    found for this gene. (provided by RefSeq, Aug 2012) 
RPS7
    Ribosomes, the organelles that catalyze protein synthesis, consist of a small 40S subunit and a large 60S subunit.
    Together these subunits are composed of 4 RNA species and approximately 80 structurally distinct proteins. This gene
    encodes a ribosomal protein that is a component of the 40S subunit. The protein belongs to the S7E family of ribosomal
    proteins. It is located in the cytoplasm. As is typical for genes encoding ribosomal proteins, there are multiple
    processed pseudogenes of this gene dispersed through the genome. (provided by RefSeq, Jul 2008) 

RPL31
    Ribosomes, the organelles that catalyze protein synthesis, consist of a small 40S subunit and a large 60S subunit.
    Together these subunits are composed of 4 RNA species and approximately 80 structurally distinct proteins. This gene
    encodes a ribosomal protein that is a component of the 60S subunit. The protein belongs to the L31E family of
    ribosomal proteins. It is located in the cytoplasm. Higher levels of expression of this gene in familial adenomatous
    polyps compared to matched normal tissues have been observed. As is typical for genes encoding ribosomal proteins,
    there are multiple processed pseudogenes of this gene dispersed through the genome. Alternatively spliced transcript
    variants encoding distinct isoforms have been found for this gene. (provided by RefSeq, Jul 2008) 
ERCC5

BPTF
DSC1
RAB1A
SH3KBP1
UBE2D1
ZFP42
MKRN1
FAF2
PDXDC1
PTBP1
RAB11B
UBC
RBM39
YOD1
