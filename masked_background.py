import cv2
import numpy as np

def detect_balls(background_file_name, file_name):

	background = cv2.imread(background_file_name)
	frame = cv2.imread(file_name)

	#Background subtraction
	diff = cv2.absdiff(frame, background)
	cv2.imwrite("diff.jpg", diff)

	#Threshold image
	min_radius = 50
	max_radius = 1000

	lower_blue = np.array([15, 15, 15])
	upper_blue = np.array([255, 255, 255])

	gray_frame = cv2.inRange(diff, lower_blue, upper_blue)
	gray_frame = cv2.medianBlur(gray_frame, 9)

	#Mask thresholded image with original image
	masked_frame = cv2.bitwise_and(frame, frame, mask = gray_frame)

	cv2.imwrite("OutputMasked.jpg", masked_frame)

	#Convert image to grayscale
	gray_frame = cv2.cvtColor(masked_frame, cv2.COLOR_BGR2GRAY)
	gray_frame = cv2.medianBlur(gray_frame,3)
	cv2.imwrite("OutputGray.jpg", gray_frame)

	#Detect circles
	balls = cv2.HoughCircles(gray_frame, cv2.cv.CV_HOUGH_GRADIENT, 5, minDist=40, param1=100, param2=70, minRadius=17, maxRadius=30)
	balls = np.round(balls[0, :]).astype("int")
	print("Detected balls")

	# #Blob detection
	# detector = cv2.SimpleBlobDetector()
	# keypoints = detector.detect(gray_frame)
	# print(keypoints)
	# im_with_keypoints = cv2.drawKeypoints(frame, keypoints, np.array([]), (0,0,255), cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
	# cv2.imwrite("blobballs.jpg", im_with_keypoints)

	#Display detected balls
	for (x, y, r) in balls:
		cv2.circle(frame, (x, y), r, (0, 255, 0), 4)

	cv2.imwrite("DetectedBalls.jpg", frame)

	return (masked_frame, balls)

def determine_color(frame, balls, radius):

	dict_balls={}

	return dict_balls


def main():

	background_file_name = 'Background_Color.jpg'
	file_name = 'StableOutput_Color4.jpg'
	(masked_frame, balls) = detect_balls(background_file_name, file_name)
	
	ball_colors = determine_color(masked_frame, balls, 22)

if __name__ == '__main__': main()