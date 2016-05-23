import os
path = "/home/iagostorch/VIDEOS_HEVC/"
lowResVideoList = ['BasketballDrill_832x480_50.yuv', 'BQMall_832x480_60.yuv', 'PartyScene_832x480_50.yuv', 'RaceHorsesC_832x480_30.yuv']
medResVideoList = ['FourPeople_1280x720_60.yuv', 'Johnny_1280x720_60.yuv', 'KristenAndSara_1280x720_60.yuv', 'SlideShow_1280x720_20.yuv']
highResVideoList = ['BasketballDrive_1920x1080_50.yuv', 'BQTerrace_1920x1080_60.yuv', 'Cactus_1920x1080_50.yuv', 'Kimono1_1920x1080_24.yuv', 'ParkScene_1920x1080_24.yuv']
ultraResVideoList = ['Traffic_2560x1600_30.yuv', 'SteamLocomotiveTrain_2560x1600_60_10bit_crop.yuv', 'NebutaFestival_2560x1600_60_10bit_crop.yuv', 'PeopleOnStreet_2560x1600_30.yuv']
kimono = ['Kimono1_1920x1080_24.yuv']
#QPList = [22, 27, 32, 37]
QPList = [37]
#PARAMETERS	lowRes:	8 13 3 2 nFrames
#			medRes:	12 20 5 6 nFrames
#			highRes: 17 30 7 11 nFrames


for video in kimono:
	for qp in QPList:
		videoName = video.split("_")[0]

		###		2X2 UNIFORM
		cmd = "./TAppEncoderStatic -c ../cfg/encoder_lowdelay_main.cfg -c ../cfg/per-sequence/" + videoName + ".cfg --CFM=1 --ECU=1 --TileUniformSpacing=1 --NumTileColumnsMinus1=1 --NumTileRowsMinus1=1 --FramesToBeEncoded=240 --QP=" + str(qp) + " --InputFile=" + path + video + " >> summary.txt"
		print cmd
		os.system(cmd)

		cmd = "mv time_perCU.txt " + videoName + "_QP" + str(qp) +  "_time_perCU.txt" #RENAME THE FILE TO CURRENT VIDEO NAME
		print cmd
		os.system(cmd)
		cmd = "mv " + videoName + "_QP" + str(qp) +  "_time_perCU.txt /home/iagostorch/ICIP2016/FAST_2X2_UNIFORM/"	#MOVE THE FILE TO RESULTS FOLDER
		print cmd
		os.system(cmd)
	
		cmd = "mv time_perTile.csv " + videoName + "_QP" + str(qp) +  "_time_perTile.csv"
		print cmd
		os.system(cmd)
		cmd = "mv " + videoName + "_QP" + str(qp) +  "_time_perTile.csv /home/iagostorch/ICIP2016/FAST_2X2_UNIFORM/"	#MOVE THE FILE TO RESULTS FOLDER
		print cmd
		os.system(cmd)

		cmd = "mv summary.txt " + videoName + "_QP" + str(qp) +  "_summary.txt"
		print cmd
		os.system(cmd)
		cmd = "mv " + videoName + "_QP" + str(qp) +  "_summary.txt /home/iagostorch/ICIP2016/FAST_2X2_UNIFORM/"
		print cmd
		os.system(cmd)

		###		2X2 OPTIMAL
		cmd = "./TAppEncoderStatic -c ../cfg/encoder_lowdelay_main.cfg -c ../cfg/per-sequence/" + videoName + ".cfg --CFM=1 --ECU=1  --TileUniformSpacing=0 --NumTileColumnsMinus1=1 --NumTileRowsMinus1=1 --TileColumnWidthArray=0 --TileRowHeightArray=0 --FramesToBeEncoded=240 --QP=" + str(qp) + " --InputFile=" + path + video + " >> summary.txt"
		print cmd
		os.system(cmd)

		cmd = "mv time_perCU.txt " + videoName + "_QP" + str(qp) +  "_time_perCU.txt" #RENAME THE FILE TO CURRENT VIDEO NAME
		print cmd
		os.system(cmd)
		cmd = "mv " + videoName + "_QP" + str(qp) +  "_time_perCU.txt /home/iagostorch/ICIP2016/FAST_2X2_OPTIMAL/"	#MOVE THE FILE TO RESULTS FOLDER
		print cmd
		os.system(cmd)
	
		cmd = "mv time_perTile.csv " + videoName + "_QP" + str(qp) +  "_time_perTile.csv"
		print cmd
		os.system(cmd)
		cmd = "mv " + videoName + "_QP" + str(qp) +  "_time_perTile.csv /home/iagostorch/ICIP2016/FAST_2X2_OPTIMAL/"	#MOVE THE FILE TO RESULTS FOLDER
		print cmd
		os.system(cmd)

		cmd = "mv summary.txt " + videoName + "_QP" + str(qp) +  "_summary.txt"
		print cmd
		os.system(cmd)
		cmd = "mv " + videoName + "_QP" + str(qp) +  "_summary.txt /home/iagostorch/ICIP2016/FAST_2X2_OPTIMAL/"
		print cmd
		os.system(cmd)
