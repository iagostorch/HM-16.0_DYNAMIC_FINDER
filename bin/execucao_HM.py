import os
path = "/home/iagostorch/VIDEOS_HEVC/"
lowResVideoList = ['BasketballDrill_832x480_50.yuv', 'BQMall_832x480_60.yuv', 'PartyScene_832x480_50.yuv', 'RaceHorsesC_832x480_30.yuv']
medResVideoList = ['FourPeople_1280x720_60.yuv', 'Johnny_1280x720_60.yuv', 'KristenAndSara_1280x720_60.yuv', 'SlideShow_1280x720_20.yuv']
highResVideoList = ['BasketballDrive_1920x1080_50.yuv', 'BQTerrace_1920x1080_60.yuv', 'Cactus_1920x1080_50.yuv', 'Kimono1_1920x1080_24.yuv']
ultraResVideoList = ['Traffic_2560x1600_30.yuv', 'SteamLocomotiveTrain_2560x1600_60_10bit_crop.yuv', 'NebutaFestival_2560x1600_60_10bit_crop.yuv', 'PeopleOnStreet_2560x1600_30.yuv']

QPList = [22, 27, 32, 37]

#PARAMETERS	lowRes:	8 13 3 2 nFrames
#			medRes:	12 20 5 6 nFrames
#			highRes: 17 30 7 11 nFrames


#------------EXAUSTIVO--------------------

for video in lowResVideoList:
	for qp in QPList:
		videoName = video.split("_")[0]

		###		1X2_EXAUSTIVO
		cmd = "./TAppEncoderStatic -c ../cfg/encoder_lowdelay_main.cfg -c ../cfg/per-sequence/" + videoName + ".cfg --TileUniformSpacing=0 --NumTileColumnsMinus1=0 --NumTileRowsMinus1=1 --TileColumnWidthArray=0 --TileRowHeightArray=0 --FramesToBeEncoded=100 --QP=" + str(qp) + " --InputFile=" + path + video + " >> summary.txt"
		print cmd
		os.system(cmd)

		cmd = "mv time_perCU.txt " + videoName + "_QP" + str(qp) +  "_time_perCU.txt" #RENAME THE FILE TO CURRENT VIDEO NAME
		print cmd
		os.system(cmd)
		cmd = "mv " + videoName + "_QP" + str(qp) +  "_time_perCU.txt /home/iagostorch/Journal/SIMULACOES/EXAUSTIVO/1X2_EXAUSTIVO/"	#MOVE THE FILE TO RESULTS FOLDER
		print cmd
		os.system(cmd)
	
		cmd = "mv time_perTile.csv " + videoName + "_QP" + str(qp) +  "_time_perTile.csv"
		print cmd
		os.system(cmd)
		cmd = "mv " + videoName + "_QP" + str(qp) +  "_time_perTile.csv /home/iagostorch/Journal/SIMULACOES/EXAUSTIVO/1X2_EXAUSTIVO/"	#MOVE THE FILE TO RESULTS FOLDER
		print cmd
		os.system(cmd)

		cmd = "mv summary.txt " + videoName + "_QP" + str(qp) +  "_summary.txt"
		print cmd
		os.system(cmd)
		cmd = "mv " + videoName + "_QP" + str(qp) +  "_summary.txt /home/iagostorch/Journal/SIMULACOES/EXAUSTIVO/1X2_EXAUSTIVO/"
		print cmd
		os.system(cmd)



for video in medResVideoList:
	for qp in QPList:
		videoName = video.split("_")[0]

		###		1X2_EXAUSTIVO
		cmd = "./TAppEncoderStatic -c ../cfg/encoder_lowdelay_main.cfg -c ../cfg/per-sequence/" + videoName + ".cfg --TileUniformSpacing=0 --NumTileColumnsMinus1=0 --NumTileRowsMinus1=1 --TileColumnWidthArray=0 --TileRowHeightArray=0 --FramesToBeEncoded=100 --QP=" + str(qp) + " --InputFile=" + path + video + " >> summary.txt"
		print cmd
		os.system(cmd)

		cmd = "mv time_perCU.txt " + videoName + "_QP" + str(qp) +  "_time_perCU.txt" #RENAME THE FILE TO CURRENT VIDEO NAME
		print cmd
		os.system(cmd)
		cmd = "mv " + videoName + "_QP" + str(qp) +  "_time_perCU.txt /home/iagostorch/Journal/SIMULACOES/EXAUSTIVO/1X2_EXAUSTIVO/"	#MOVE THE FILE TO RESULTS FOLDER
		print cmd
		os.system(cmd)
	
		cmd = "mv time_perTile.csv " + videoName + "_QP" + str(qp) +  "_time_perTile.csv"
		print cmd
		os.system(cmd)
		cmd = "mv " + videoName + "_QP" + str(qp) +  "_time_perTile.csv /home/iagostorch/Journal/SIMULACOES/EXAUSTIVO/1X2_EXAUSTIVO/"	#MOVE THE FILE TO RESULTS FOLDER
		print cmd
		os.system(cmd)

		cmd = "mv summary.txt " + videoName + "_QP" + str(qp) +  "_summary.txt"
		print cmd
		os.system(cmd)
		cmd = "mv " + videoName + "_QP" + str(qp) +  "_summary.txt /home/iagostorch/Journal/SIMULACOES/EXAUSTIVO/1X2_EXAUSTIVO/"
		print cmd
		os.system(cmd)



for video in highResVideoList:
	for qp in QPList:
		videoName = video.split("_")[0]

		###		1X2_EXAUSTIVO
		cmd = "./TAppEncoderStatic -c ../cfg/encoder_lowdelay_main.cfg -c ../cfg/per-sequence/" + videoName + ".cfg --TileUniformSpacing=0 --NumTileColumnsMinus1=0 --NumTileRowsMinus1=1 --TileColumnWidthArray=0 --TileRowHeightArray=0 --FramesToBeEncoded=100 --QP=" + str(qp) + " --InputFile=" + path + video + " >> summary.txt"
		print cmd
		os.system(cmd)

		cmd = "mv time_perCU.txt " + videoName + "_QP" + str(qp) +  "_time_perCU.txt" #RENAME THE FILE TO CURRENT VIDEO NAME
		print cmd
		os.system(cmd)
		cmd = "mv " + videoName + "_QP" + str(qp) +  "_time_perCU.txt /home/iagostorch/Journal/SIMULACOES/EXAUSTIVO/1X2_EXAUSTIVO/"	#MOVE THE FILE TO RESULTS FOLDER
		print cmd
		os.system(cmd)
	
		cmd = "mv time_perTile.csv " + videoName + "_QP" + str(qp) +  "_time_perTile.csv"
		print cmd
		os.system(cmd)
		cmd = "mv " + videoName + "_QP" + str(qp) +  "_time_perTile.csv /home/iagostorch/Journal/SIMULACOES/EXAUSTIVO/1X2_EXAUSTIVO/"	#MOVE THE FILE TO RESULTS FOLDER
		print cmd
		os.system(cmd)

		cmd = "mv summary.txt " + videoName + "_QP" + str(qp) +  "_summary.txt"
		print cmd
		os.system(cmd)
		cmd = "mv " + videoName + "_QP" + str(qp) +  "_summary.txt /home/iagostorch/Journal/SIMULACOES/EXAUSTIVO/1X2_EXAUSTIVO/"
		print cmd
		os.system(cmd)


#------------UNIFORM---------------------UNIFORM----------------------UNIFORM----------------------------

for video in lowResVideoList:
	for qp in QPList:
		videoName = video.split("_")[0]

		###		1X2_UNIFORM
		cmd = "./TAppEncoderStatic -c ../cfg/encoder_lowdelay_main.cfg -c ../cfg/per-sequence/" + videoName + ".cfg --TileUniformSpacing=1 --NumTileColumnsMinus1=0 --NumTileRowsMinus1=1 --FramesToBeEncoded=100 --QP=" + str(qp) + " --InputFile=" + path + video + " >> summary.txt"
		print cmd
		os.system(cmd)

		cmd = "mv time_perCU.txt " + videoName + "_QP" + str(qp) +  "_time_perCU.txt" #RENAME THE FILE TO CURRENT VIDEO NAME
		print cmd
		os.system(cmd)
		cmd = "mv " + videoName + "_QP" + str(qp) +  "_time_perCU.txt /home/iagostorch/Journal/SIMULACOES/UNIFORM/1X2_UNIFORM/"	#MOVE THE FILE TO RESULTS FOLDER
		print cmd
		os.system(cmd)
	
		cmd = "mv time_perTile.csv " + videoName + "_QP" + str(qp) +  "_time_perTile.csv"
		print cmd
		os.system(cmd)
		cmd = "mv " + videoName + "_QP" + str(qp) +  "_time_perTile.csv /home/iagostorch/Journal/SIMULACOES/UNIFORM/1X2_UNIFORM/"	#MOVE THE FILE TO RESULTS FOLDER
		print cmd
		os.system(cmd)

		cmd = "mv summary.txt " + videoName + "_QP" + str(qp) +  "_summary.txt"
		print cmd
		os.system(cmd)
		cmd = "mv " + videoName + "_QP" + str(qp) +  "_summary.txt /home/iagostorch/Journal/SIMULACOES/UNIFORM/1X2_UNIFORM/"
		print cmd
		os.system(cmd)



for video in medResVideoList:
	for qp in QPList:
		videoName = video.split("_")[0]

		###		1X2_UNIFORM
		cmd = "./TAppEncoderStatic -c ../cfg/encoder_lowdelay_main.cfg -c ../cfg/per-sequence/" + videoName + ".cfg --TileUniformSpacing=1 --NumTileColumnsMinus1=0 --NumTileRowsMinus1=1 --FramesToBeEncoded=100 --QP=" + str(qp) + " --InputFile=" + path + video + " >> summary.txt"
		print cmd
		os.system(cmd)

		cmd = "mv time_perCU.txt " + videoName + "_QP" + str(qp) +  "_time_perCU.txt" #RENAME THE FILE TO CURRENT VIDEO NAME
		print cmd
		os.system(cmd)
		cmd = "mv " + videoName + "_QP" + str(qp) +  "_time_perCU.txt /home/iagostorch/Journal/SIMULACOES/UNIFORM/1X2_UNIFORM/"	#MOVE THE FILE TO RESULTS FOLDER
		print cmd
		os.system(cmd)
	
		cmd = "mv time_perTile.csv " + videoName + "_QP" + str(qp) +  "_time_perTile.csv"
		print cmd
		os.system(cmd)
		cmd = "mv " + videoName + "_QP" + str(qp) +  "_time_perTile.csv /home/iagostorch/Journal/SIMULACOES/UNIFORM/1X2_UNIFORM/"	#MOVE THE FILE TO RESULTS FOLDER
		print cmd
		os.system(cmd)

		cmd = "mv summary.txt " + videoName + "_QP" + str(qp) +  "_summary.txt"
		print cmd
		os.system(cmd)
		cmd = "mv " + videoName + "_QP" + str(qp) +  "_summary.txt /home/iagostorch/Journal/SIMULACOES/UNIFORM/1X2_UNIFORM/"
		print cmd
		os.system(cmd)



for video in highResVideoList:
	for qp in QPList:
		videoName = video.split("_")[0]

		###		1X2_UNIFORM
		cmd = "./TAppEncoderStatic -c ../cfg/encoder_lowdelay_main.cfg -c ../cfg/per-sequence/" + videoName + ".cfg --TileUniformSpacing=1 --NumTileColumnsMinus1=0 --NumTileRowsMinus1=1 --FramesToBeEncoded=100 --QP=" + str(qp) + " --InputFile=" + path + video + " >> summary.txt"
		print cmd
		os.system(cmd)

		cmd = "mv time_perCU.txt " + videoName + "_QP" + str(qp) +  "_time_perCU.txt" #RENAME THE FILE TO CURRENT VIDEO NAME
		print cmd
		os.system(cmd)
		cmd = "mv " + videoName + "_QP" + str(qp) +  "_time_perCU.txt /home/iagostorch/Journal/SIMULACOES/UNIFORM/1X2_UNIFORM/"	#MOVE THE FILE TO RESULTS FOLDER
		print cmd
		os.system(cmd)
	
		cmd = "mv time_perTile.csv " + videoName + "_QP" + str(qp) +  "_time_perTile.csv"
		print cmd
		os.system(cmd)
		cmd = "mv " + videoName + "_QP" + str(qp) +  "_time_perTile.csv /home/iagostorch/Journal/SIMULACOES/UNIFORM/1X2_UNIFORM/"	#MOVE THE FILE TO RESULTS FOLDER
		print cmd
		os.system(cmd)

		cmd = "mv summary.txt " + videoName + "_QP" + str(qp) +  "_summary.txt"
		print cmd
		os.system(cmd)
		cmd = "mv " + videoName + "_QP" + str(qp) +  "_summary.txt /home/iagostorch/Journal/SIMULACOES/UNIFORM/1X2_UNIFORM/"
		print cmd
		os.system(cmd)
