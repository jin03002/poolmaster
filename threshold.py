import cv2
import os
# from pykinect2 import PyKinectV2
# from pykinect2.PyKinectV2 import *
# from pykinect2 import PyKinectRuntime
import numpy as np


def main():

	#Initialize Kinect
	# kinect = PyKinectRuntime.PyKinectRuntime(PyKinectV2.FrameSourceTypes_Color | PyKinectV2.FrameSourceTypes_Body)

	# #Get one color picture frame
	# frame=None
	# while(True):
	# 	if(kinect.has_new_color_frame()):
	# 		frame = kinect.get_last_color_frame()
	# 		print(frame.view())
	# 		break

	# #Convert to opencv
	# frame = frame.reshape((1080, 1920, 4))

	frame = cv2.imread('Output3.jpg')

	#Create opencv output
	output = frame.copy()

	#Output to file for testing
	cv2.imwrite("Output.jpg", frame)

	#56, 182, 241
	#20, 77, 144

	inc=125
	ind=1

	for i in range(5,256,inc):
		for j in range(5,256,inc):
			for k in range(5,256,inc):
				for l in range(i+inc,256,inc):
					for m in range(j+inc,256,inc):
						for n in range(k+inc,256,inc):
							#Hough circle detection using greyscale img
							min_radius = 50
							max_radius = 1000

							lower_blue = np.array([i,j,k])
							upper_blue = np.array([l,m,n])

							gray_frame = cv2.inRange(frame, lower_blue, upper_blue)
							gray_frame = cv2.medianBlur(gray_frame,21)
							cv2.imwrite("OutputGray"+str(ind)+".jpg", gray_frame)

							balls = cv2.HoughCircles(gray_frame, cv2.cv.CV_HOUGH_GRADIENT, 1, minDist=50)
							# balls = cv2.HoughCircles(gray_frame, cv2.cv.CV_HOUGH_GRADIENT, 1, circles=21, minDist=150, param1=100, param2=100, minRadius=40, maxRadius=200)

							#Display detected balls

							if(balls!=None):
								balls = np.round(balls[0, :]).astype("int")

								for (x, y, z) in balls:
									cv2.circle(frame, (x, y), z, (0, 255, 0), 4)

								cv2.imwrite("DetectedBalls"+str(ind)+".jpg", frame )
								cv2.waitKey(0)

							ind+=1

if __name__=="__main__": main()