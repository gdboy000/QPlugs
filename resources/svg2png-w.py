import sys
from cairosvg import svg2png
import cv2
import os


if __name__ == '__main__':
    if len(sys.argv) < 2:
        raise Exception("参数不足 use -help")
    file_url = sys.argv[1]
    if file_url == '-help':
        print("USE python3 svg2png-w fold/demo.svg\n\tGet fold/demo-w.png in same fold!")
    strs = file_url.split(".")
    if strs[1] != "svg":
        raise Exception("输入文件格式错误 -help")

    # 将SVG转换为PNG
    svg2png(url=file_url, write_to="tmp.png")


    # 将PIL图片转换为OpenCV格式
    cv_img = cv2.imread("tmp.png",cv2.IMREAD_UNCHANGED)

    if cv_img is None:
        raise Exception("图像读取失败")
    else:
        print("--svg read Success")

    # 分离各个通道
    b, g, r, a = cv2.split(cv_img)

    # 显示各个通道
    # for i in a:
    #     print(i)
    img_2 = cv_img.copy()
    for y in range(cv_img.shape[0]):
        for x in range(cv_img.shape[1]):
            # 获取当前像素的Alpha通道值
            alpha_value = cv_img[y, x, 3]
            # 如果Alpha通道值为255，则将BGR通道值设置为255
            if alpha_value > 0:
                #f = alpha_value / 255;
                img_2[y, x, 0] = alpha_value  # Blue
                img_2[y, x, 1] = alpha_value  # Green
                img_2[y, x, 2] = alpha_value  # Red
                img_2[y, x, 3] = alpha_value
            else:
                img_2[y, x, 3] = 0
            #img_2[y, x, 3] = 255 - alpha_value
    #删除文件
    os.remove("tmp.png")

    if cv2.imwrite(strs[0]+"-w.png", img_2):
        print("--change Success")
    else:
        print("--change Error")
    # inverted_image = 255 - cv_img
    # # 例如，显示图片
    # cv2.namedWindow('Image',1)
    # # cv2.imshow('Image_r', inverted_image)
    # cv2.imshow('Image', cv_img)
    #cv2.waitKey(0)

