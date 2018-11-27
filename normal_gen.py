import numpy as np
from PIL import Image
import pprint
pp=pprint.PrettyPrinter(4)
file="1102_d.jpg"
img=Image.open(file)
img=img.convert("L")
img_arr=np.zeros(shape=(img.width,img.height))
s=np.zeros(3)
t=np.zeros(3)
s[0]=1
t[1]=1
N=np.zeros(shape=(img.height,img.width,3))
for x in range(img.width):
	for y in range(img.height):
		img_arr[x][y]=img.getpixel((x,y))/255
for x in range(img.width):
	for y in range(img.height):
		print(x,y)
		if(x==0 or x==(img.width-1)):
			s[2]=img_arr[x][y]
		else:
			s[2]=img_arr[x+1][y]-img_arr[x-1][y]
		if(y==0 or y==(img.height-1)):
			t[2]=img_arr[x][y]
		else:
			t[2]=img_arr[x][y+1]-img_arr[x][y-1]
		cross=np.cross(s,t)
		cross= cross/np.linalg.norm(cross)
		# print(cross)
		N[y][x]=cross*127+128
		# print(N[x][y])

img_normal=Image.fromarray(np.uint8(N))
img_normal.save("normal "+file)

# pp.pprint(img_normal)
# img_arr.show()
# img_normal.show()