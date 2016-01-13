import cv2
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

	frame = cv2.imread('Output4.jpg')

	#Create opencv output
	output = frame.copy()

	#Output to file for testing
	cv2.imwrite("Output.jpg", frame)

	#56, 182, 241
	#20, 77, 144

	#Hough circle detection using greyscale img
	min_radius = 50
	max_radius = 1000

	gray_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
	# gray_frame = cv2.medianBlur(gray_frame,21)
	cv2.imwrite("OutputGray.jpg", gray_frame)

	balls = cv2.HoughCircles(gray_frame, cv2.cv.CV_HOUGH_GRADIENT, 2.2 , minDist=40, minRadius=40, maxRadius=1400)
	# balls = cv2.HoughCircles(gray_frame, cv2.cv.CV_HOUGH_GRADIENT, 1.9, circles=21, minDist=150, param1=100, param2=100, minRadius=80, maxRadius=200)

	print("Detected balls")
	print(balls)
	balls = np.round(balls[0, :]).astype("int")

	#Display detected balls
	for (x, y, z) in balls:
		cv2.circle(frame, (x, y), z, (0, 255, 0), 4)

	cv2.imwrite("DetectedBalls.jpg", frame )
	cv2.imshow("output", frame)
	cv2.waitKey(0)

if __name__=="__main__": main()