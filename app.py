from flask import Flask, send_file, jsonify
from flask_cors import CORS
from PIL import Image
import subprocess
import os

app = Flask(__name__)
CORS(app)

# Absolute path to raytrace.exe inside build/Release/
RAYTRACE_EXE = os.path.join(os.getcwd(), 'build', 'raytrace')

# Path to output image
IMAGE_PATH = os.path.join(os.getcwd(), 'static', 'im.ppm')

PPM_PATH = "static/im.ppm"
PNG_PATH = "static/im.png"

@app.route('/render')
def render_image():
    try:
        result = subprocess.run([RAYTRACE_EXE], capture_output=True, text=True)

        if result.returncode != 0:
            return jsonify({'error': 'Raytracer failed', 'details': result.stderr}), 500

        with Image.open(PPM_PATH) as img:
            img.save(PNG_PATH, format="PNG")

        return jsonify({'message': 'Image rendered successfully'})
    except Exception as e:
        return jsonify({'error': str(e)}), 500


@app.route('/image')
def get_image():
    if os.path.exists(PNG_PATH):
        return send_file(PNG_PATH, mimetype='image/png')
    else:
        return jsonify({'error': 'Image not found'}), 404


if __name__ == '__main__':
    app.run(debug=True)
