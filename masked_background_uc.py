import cv2
import numpy as np
import csv
import math
import copy

def detect_balls(background_file_name, file_name):

	background = cv2.imread(background_file_name)
	frame = cv2.imread(file_name)
	oframe = copy.copy(frame)

	#Background subtraction
	diff = cv2.absdiff(frame, background)
	cv2.imwrite("3OutputDiff.jpg", diff)

	#Threshold image
	min_radius = 50
	max_radius = 1000

	lower_blue = np.array([6, 6, 6])
	upper_blue = np.array([255, 255, 255])

	gray_frame = cv2.inRange(diff, lower_blue, upper_blue)

	cv2.imwrite("4OutputMask.jpg", gray_frame)

	gray_frame = cv2.medianBlur(gray_frame, 9)

	#Mask thresholded image with original image
	masked_frame = cv2.bitwise_and(frame, frame, mask = gray_frame)

	cv2.imwrite("5OutputMasked.jpg", masked_frame)

	#Convert image to grayscale
	gray_frame = cv2.cvtColor(masked_frame, cv2.COLOR_BGR2GRAY)
	gray_frame = cv2.medianBlur(gray_frame,3)
	cv2.imwrite("6OutputGray.jpg", gray_frame)

	#Detect circles
	balls = cv2.HoughCircles(gray_frame, cv2.cv.CV_HOUGH_GRADIENT, 5, minDist=50, param1=100, param2=70, minRadius=17, maxRadius=30)
	balls = np.round(balls[0, :]).astype("int")

	#Display detected balls
	ind = 1
	for (x, y, r) in balls:
		cv2.circle(frame, (x, y), r, (0, 255, 0), 4)

	cv2.imwrite("7DetectedBalls.jpg", frame)

	return (oframe, masked_frame, balls)


def detect_color(frame, balls, radius):

	dict_balls={}
	hsv_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
 
	cv2.imwrite("OutputHue.jpg", hsv_frame)
	frame_copy = copy.copy(frame)

	saturation_frame = hsv_frame[0:, 0:, 2::3]
	
	cv2.imwrite("OutputSaturation.jpg", saturation_frame)

	# print(saturation_frame)

	ind=1
	for (x, y, r) in balls:

		total_non_white=0
		total_white=0
		hue=0

		# for a in range(x-radius, x+radius+1):
		# 	for b in range(y-radius, y+radius+1):
		# 		if((x-a)*(x-a)+(y-b)*(y-b))

		cv2.circle(frame_copy, (x, y), radius, (255, 0, 0), 4)
		ind+=1

	cv2.imwrite("OutputDetectColor.jpg", frame_copy)

	return dict_balls


def find_white_ball(frame, balls):
	total_rgb=[]
	for (x, y, r) in balls:
		rgb_value = 0
		for a in range(x-r, x+r):
			for b in range(y-r, y+r):
				if((a-x)^2 + (b-y)^2 < r^2):
					rgb_value += frame[b,a,0]
					rgb_value += frame[b,a,1]
					rgb_value += frame[b,a,2]

		n = [rgb_value, x, y, r]
		total_rgb.append(n)

	total_rgb = np.array(total_rgb)
	total_rgb=total_rgb[np.argsort(total_rgb[:, 0])]

	(a, wx, wy, r) = total_rgb[len(balls)-1]
	# print(wx,wy)
	cv2.circle(frame, (wx, wy), r, (255, 0, 0), 4)
	cv2.imwrite("8OutputDetectWhite.jpg", frame)
	
	sort = [i[1:] for i in total_rgb]
	return sort[::-1]


def find_pool_table(frame, balls, ctl, cbr):

	pixl = cbr[0] - ctl[0]
	pixw = cbr[1] - ctl[1]

	pooll = 2.236
	poolw = 1.116
	
	l = pixl/pooll
	w = pixw/poolw


	# ctl = (75,103)
	# ctr = (1859,103)
	# cbl = (131,965)
	# cbr = (1813,961)
	

	# avgx = ctl[0]+ctr[0]+cbl[0]+cbr[0]
	# avgx/=4
	# avgy = ctl[1]+ctr[1]+cbl[1]+cbr[1]
	# avgy/=4
	# cv2.line(frame, ctl, ctr, (255, 0, 0), 4)
	# cv2.line(frame, ctr, cbr, (255, 0, 0), 4)
	# cv2.line(frame, cbl, cbr, (255, 0, 0), 4)
	# cv2.line(frame, cbl, ctl, (255, 0, 0), 4)
	cv2.rectangle(frame, ctl, cbr, (255, 0, 0), 4)
	nballs=[]

	for (x,y,r) in balls:
		if x<cbr[0] and x>ctl[0] and y<cbr[1] and y>ctl[1]:
			nballs.append((x,y,r))

	for i in range(len(nballs)):
		(x, y, r) = nballs[i]
		color= (0, 255, 0)
		# if i is 0:
		# 	color =  ( 255, 0, 0)
		cv2.circle(frame, (x, y), r, color, 4)
	#for (x, y, r) in shifted_coords:
	# 	cv2.circle(frame, int(x), int(y), int(r), (0, 255, 0), 4)

	return nballs


def shift(frame, balls, ctl, cbr):

	pixl = cbr[0] - ctl[0]
	pixw = cbr[1] - ctl[1]

	pooll = 2.236
	poolw = 1.116
	
	l = pixl/pooll
	w = pixw/poolw


	# ctl = (75,103)
	# ctr = (1859,103)
	# cbl = (131,965)
	# cbr = (1813,961)
	

	# avgx = ctl[0]+ctr[0]+cbl[0]+cbr[0]
	# avgx/=4
	# avgy = ctl[1]+ctr[1]+cbl[1]+cbr[1]
	# avgy/=4
	# cv2.line(frame, ctl, ctr, (255, 0, 0), 4)
	# cv2.line(frame, ctr, cbr, (255, 0, 0), 4)
	# cv2.line(frame, cbl, cbr, (255, 0, 0), 4)
	# cv2.line(frame, cbl, ctl, (255, 0, 0), 4)

	#cv2.imwrite("OutputDetectWhite2.jpg", frame)

	shifted_coords=[]
	for (x,y,r) in balls:
		if x<cbr[0] and x>ctl[0] and y<cbr[1] and y>ctl[1]:
			shifted_coords.append(((x-ctl[0])/l,(y-ctl[1])/w, r))

	# for (x, y, r) in shifted_coords:
	# 	cv2.circle(frame, int(x), int(y), int(r), (0, 255, 0), 4)

	#cv2.imwrite("DetectedBalls2.jpg", frame)

	return shifted_coords


def visualize(original_frame, background, original_coords, shifted_coords, ctl, cbr):

	pixl = cbr[0] - ctl[0]
	pixw = cbr[1] - ctl[1]

	pooll = 2.236
	poolw = 1.116
	
	l = pixl/pooll
	w = pixw/poolw

	rad = (l+w)/2

	#print(shifted_coords)

	for i in range(len(shifted_coords)):
		(nx, ny, nr) = shifted_coords[i]
		color= (0, 255, 0)
		if i is 0:
			color =  ( 255, 0, 0)
		nx=float(nx)
		ny=float(ny)
		nr=float(nr)
		x=nx*l

		x=int(x)
		y=int(ny*w)
		r=int(nr*rad)

		nx=original_coords[i][0]
		ny=original_coords[i][1]

		print(x,y)

		if(x < 1800 and y < 900):
			current_ball = original_frame[(ny-r-1) : (ny+r+2), (nx-r-1) : (nx+r+2)]
			#cv2.imwrite("Testing"+str(i+1)+".jpg",current_ball)
			background[(y+ctl[1]-r-1) : (y+ctl[1]+r+2), (x+ctl[0]-r-1) : (x+ctl[0]+r+2)] = current_ball

			cv2.circle(original_frame , (nx, ny), r, color, 4)
			#cv2.circle(background, (x+ctl[0], y+ctl[1]), r, color, 4)
	
	cv2.imwrite("10visualize.jpg", background)
	#cv2.imwrite("test.jpg", original_frame)

def drawL(frame, nballs, angle1, angle2):
	length = 10000
	P1 = (nballs[0][0], nballs[0][1])

	P2 = (int(round(P1[0] + length * math.cos(angle1 * math.pi / 180.0))),
		  int(round(P1[1] + length * math.sin(angle1 * math.pi / 180.0))))

	P3 = (int(round(P1[0] + length * math.cos(angle2 * math.pi / 180.0))),
		  int(round(P1[1] + length * math.sin(angle2 * math.pi / 180.0))))

	cv2.line(frame, P1, P2, (255, 0, 0), 4)
	cv2.line(frame, P1, P3, (255, 0, 0), 4)

	cv2.imwrite("9line.jpg", frame)

def main():
	ctl = (45,53) # just have to change ctl and cbr
	# ctr = (1859,103)
	# cbl = (103,965)
	cbr = (1832,951)

	# ctl = (105,103)
	# ctr = (1859,103)
	# cbl = (131,965)
	# cbr = (1843,961)

	background_file_name = 'UCBackground.jpg'
	background = cv2.imread(background_file_name)
	file_name = 'UCOutput1.jpg'
	(frame, masked_frame, balls) = detect_balls(background_file_name, file_name)
	oframe = copy.copy(frame)	

	nballs = find_pool_table(frame, balls, ctl, cbr)

	nballs = find_white_ball(frame, nballs)
	shifted_coords = shift(frame, nballs, ctl, cbr)

	# with open('OutputBalls.csv', 'wb') as f:
	# 	writer=csv.writer(f)
	# 	writer.writerows(nballs)

	with open('OutputShiftedCoords.csv', 'wb') as f:
		writer=csv.writer(f)
		writer.writerows(shifted_coords)

	sc = []
	with open('out_state.csv', 'rb') as csvfile:
		reader = csv.reader(csvfile)
		for row in reader:
			sc.append(row)

	angle1 = int(sc[0][0])
	angle2 = int(sc[1][0])
	sc=sc[2:]

	visualize(oframe, background, nballs, sc, ctl, cbr)
	drawL(oframe, nballs, angle1, angle2)

if __name__ == '__main__': main()