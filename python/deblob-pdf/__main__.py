from pathlib import Path
from PIL import Image
from reportlab.pdfgen import canvas
from reportlab.lib.pagesizes import A4
import io
import os
import sys

RAW_DIR = Path("/mnt/data/raw")
PNG_DIR = Path("/mnt/data/png")
OUTPUT_DIR = Path("/mnt/data")
PNG_DIR.mkdir(parents=True, exist_ok=True)

print("Finding target file ...")
if not RAW_DIR.exists():
    print(f"Error: '{RAW_DIR}' directory not found")
    print("---- FALLBACK ----")
    print(f"Creating the '{RAW_DIR}' directory...")
    RAW_DIR.mkdir(parents=True, exist_ok=True)
    print(f"Directory '{RAW_DIR}' created. ADD raw files here")

files = [p for p in RAW_DIR.iterdir() if p.is_file()]
if not files:
    print(f"No files found in '{RAW_DIR}'")
    sys.exit(1)

pdf_file = OUTPUT_DIR / "output.pdf"
c = canvas.Canvas(str(pdf_file), pagesize=A4)

print("Working in:", os.getcwd())
print(f"Found {len(files)} file(s) in '{RAW_DIR}' to process.")

for file in files:
    with open(file, "rb") as f:
        blob_data = f.read()

    image = Image.open(io.BytesIO(blob_data))

    try:
        image = ImageOps.exif_transpose(image)
    except Exception:
        pass
    if image.mode in ("RGBA", "P"):
        image = image.convert("RGB")

    temp_file = PNG_DIR / (file.name + ".png")
    image.save(temp_file)

    iw, ih = image.size
    pw, ph = A4
    scale = min(pw / iw, ph / ih) * 0.9
    new_w, new_h = iw * scale, ih * scale

    x = (pw - new_w) / 2
    y = (ph - new_h) / 2

    c.drawImage(str(temp_file), x, y, width=new_w, height=new_h)
    c.showPage()

c.save()
print("---- PDF created ----")
print(f"File name: {pdf_file}")
print(f"Location: {os.path.abspath(pdf_file)}")
