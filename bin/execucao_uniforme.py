import os
path = "/home/iagostorch/VIDEOS_HEVC/"
lowResVideoList = ['BasketballDrill_832x480_50.yuv', 'BQMall_832x480_60.yuv', 'PartyScene_832x480_50.yuv', 'RaceHorsesC_832x480_30.yuv']
medResVideoList = ['FourPeople_1280x720_60.yuv', 'Johnny_1280x720_60.yuv', 'KristenAndSara_1280x720_60.yuv', 'SlideShow_1280x720_20.yuv']
highResVideoList = ['BasketballDrive_1920x1080_50.yuv', 'BQTerrace_1920x1080_60.yuv', 'Cactus_1920x1080_50.yuv', 'Kimono1_1920x1080_24.yuv']
ultraResVideoList = ['Traffic_2560x1600_30_crop.yuv', 'SteamLocomotiveTrain_2560x1600_60_10bit_crop.yuv']
#PARAMETERS	lowRes:	8 13 3 2 nFrames
#			medRes:	12 20 5 6 nFrames
#			highRes: 17 30 7 11 nFrames

QPList = [22, 27, 32, 37]

#######################################			3X3			##############################################3

################################################	HIGH 	################################33
for video in highResVideoList:
	for qp in QPList:
		videoName = video.split("_")[0]

		cmd = "./TAppEncoderStatic -c ../cfg/encoder_lowdelay_main.cfg -c ../cfg/per-sequence/" + videoName + ".cfg --TileUniformSpacing=1 --NumTileColumnsMinus1=2 --NumTileRowsMinus1=2  --FramesToBeEncoded=2 --QP=" + str(qp) + " --InputFile=" + path + video + " >> summary.txt"
		print cmd
		os.system(cmd)

		cmd = "mv time_perCU.txt " + videoName + "_QP" + str(qp) +  "_time_perCU.txt" #RENAME THE FILE TO CURRENT VIDEO NAME
		print cmd
		os.system(cmd)
		cmd = "mv " + videoName + "_QP" + str(qp) +  "_time_perCU.txt /home/iagostorch/ICIP2016/SIMULACOES_3X3_UNIFORM/"	#MOVE THE FILE TO RESULTS FOLDER
		print cmd
		os.system(cmd)
	
		cmd = "mv time_perTile.csv " + videoName + "_QP" + str(qp) +  "_time_perTile.csv"
		print cmd
		os.system(cmd)
		cmd = "mv " + videoName + "_QP" + str(qp) +  "_time_perTile.csv /home/iagostorch/ICIP2016/SIMULACOES_3X3_UNIFORM/"	#MOVE THE FILE TO RESULTS FOLDER
		print cmd
		os.system(cmd)

		cmd = "mv summary.txt " + videoName + "_QP" + str(qp) +  "_summary.txt"
		print cmd
		os.system(cmd)
		cmd = "mv " + videoName + "_QP" + str(qp) +  "_summary.txt /home/iagostorch/ICIP2016/SIMULACOES_3X3_UNIFORM/"
		print cmd
		os.system(cmd)


################################################	HIGH 	################################33
for video in medResVideoList:
	for qp in QPList:
		videoName = video.split("_")[0]

		cmd = "./TAppEncoderStatic -c ../cfg/encoder_lowdelay_main.cfg -c ../cfg/per-sequence/" + videoName + ".cfg --TileUniformSpacing=1 --NumTileColumnsMinus1=2 --NumTileRowsMinus1=2  --FramesToBeEncoded=2 --QP=" + str(qp) + " --InputFile=" + path + video + " >> summary.txt"
		print cmd
		os.system(cmd)

		cmd = "mv time_perCU.txt " + videoName + "_QP" + str(qp) +  "_time_perCU.txt" #RENAME THE FILE TO CURRENT VIDEO NAME
		print cmd
		os.system(cmd)
		cmd = "mv " + videoName + "_QP" + str(qp) +  "_time_perCU.txt /home/iagostorch/ICIP2016/SIMULACOES_3X3_UNIFORM/"	#MOVE THE FILE TO RESULTS FOLDER
		print cmd
		os.system(cmd)
	
		cmd = "mv time_perTile.csv " + videoName + "_QP" + str(qp) +  "_time_perTile.csv"
		print cmd
		os.system(cmd)
		cmd = "mv " + videoName + "_QP" + str(qp) +  "_time_perTile.csv /home/iagostorch/ICIP2016/SIMULACOES_3X3_UNIFORM/"	#MOVE THE FILE TO RESULTS FOLDER
		print cmd
		os.system(cmd)

		cmd = "mv summary.txt " + videoName + "_QP" + str(qp) +  "_summary.txt"
		print cmd
		os.system(cmd)
		cmd = "mv " + videoName + "_QP" + str(qp) +  "_summary.txt /home/iagostorch/ICIP2016/SIMULACOES_3X3_UNIFORM/"
		print cmd
		os.system(cmd)


################################################	HIGH 	################################33
for video in lowResVideoList:
	for qp in QPList:
		videoName = video.split("_")[0]

		cmd = "./TAppEncoderStatic -c ../cfg/encoder_lowdelay_main.cfg -c ../cfg/per-sequence/" + videoName + ".cfg --TileUniformSpacing=1 --NumTileColumnsMinus1=2 --NumTileRowsMinus1=2  --FramesToBeEncoded=2 --QP=" + str(qp) + " --InputFile=" + path + video + " >> summary.txt"
		print cmd
		os.system(cmd)

		cmd = "mv time_perCU.txt " + videoName + "_QP" + str(qp) +  "_time_perCU.txt" #RENAME THE FILE TO CURRENT VIDEO NAME
		print cmd
		os.system(cmd)
		cmd = "mv " + videoName + "_QP" + str(qp) +  "_time_perCU.txt /home/iagostorch/ICIP2016/SIMULACOES_3X3_UNIFORM/"	#MOVE THE FILE TO RESULTS FOLDER
		print cmd
		os.system(cmd)
	
		cmd = "mv time_perTile.csv " + videoName + "_QP" + str(qp) +  "_time_perTile.csv"
		print cmd
		os.system(cmd)
		cmd = "mv " + videoName + "_QP" + str(qp) +  "_time_perTile.csv /home/iagostorch/ICIP2016/SIMULACOES_3X3_UNIFORM/"	#MOVE THE FILE TO RESULTS FOLDER
		print cmd
		os.system(cmd)

		cmd = "mv summary.txt " + videoName + "_QP" + str(qp) +  "_summary.txt"
		print cmd
		os.system(cmd)
		cmd = "mv " + videoName + "_QP" + str(qp) +  "_summary.txt /home/iagostorch/ICIP2016/SIMULACOES_3X3_UNIFORM/"
		print cmd
		os.system(cmd)