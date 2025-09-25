from reportlab.pdfgen import canvas
from reportlab.lib.pagesizes import A4
from PIL import Image
import io
import os

base_dir    = os.path.dirname(os.path.abspath(__file__))  
raw_dir     = os.path.join(base_dir, "mnt", "data", "raw") 
png_dir     = os.path.join(base_dir, "mnt", "data", "png") 
output_dir  = os.path.join(base_dir, "mnt", "data")  

os.makedirs(png_dir, exist_ok=True)

print(f"Finding target file ...")
if not os.path.exists(raw_dir):
    print(f"Error: '{raw_dir}' directory not found")
    print(f"---- FALLBACK ----")
    print(f"Creating the '{raw_dir}' directory...")
    os.makedirs(raw_dir, exist_ok=True)
    print(f"Directory '{raw_dir}' created. ADD raw files here")

files = [os.path.join(raw_dir, f) for f in os.listdir(raw_dir) if os.path.isfile(os.path.join(raw_dir, f))]

if not files:
    print(f"No files found in '{raw_dir}'")
    exit(1)

pdf_file = os.path.join(output_dir, "output.pdf")
c = canvas.Canvas(pdf_file, pagesize=A4)

print("Working in:", os.getcwd())
print(f"Found {len(files)} file(s) in '{raw_dir}' to process.")

for file in files:
    with open(file, "rb") as f:
        blob_data = f.read()

    image = Image.open(io.BytesIO(blob_data))

    temp_file = os.path.join(png_dir, os.path.basename(file) + ".png")
    image.save(temp_file)

    iw, ih = image.size
    pw, ph = A4
    scale = min(pw / iw, ph / ih) * 0.9
    new_w, new_h = iw * scale, ih * scale

    x = (pw - new_w) / 2
    y = (ph - new_h) / 2

    c.drawImage(temp_file, x, y, width=new_w, height=new_h)
    c.showPage()

c.save()
print("---- PDF created ----")
print(f"File name: {pdf_file}")
print(f"Location: {os.path.abspath(pdf_file)}")
