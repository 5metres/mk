from PIL import Image
import os

def process_image(image_path):
    try:
        # 读取图像
        image = Image.open(image_path)
    except IOError:
        return "Error: Cannot read image at " + image_path
    
    # 将图像转换为灰度图
    gray = image.convert('L')
    
    # 构造输出文件路径（在原文件名后加 _gray）
    base, ext = os.path.splitext(image_path)
    output_path = base + "_gray" + ext
    
    # 保存处理后的图像
    gray.save(output_path)
    return output_path
