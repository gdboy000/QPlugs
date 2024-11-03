import cv2

img = cv2.imread("qr-code-w.png",cv2.IMREAD_UNCHANGED)
#print(img.shape[0],img.shape[1])
img2 = cv2.resize(img,(20,20))
cv2.imwrite("qr-code-w.png",img2)