from flask import Flask, request, send_file, jsonify
from flask_cors import CORS
from PIL import Image
import subprocess
import os
import json

app = Flask(__name__)
CORS(app)

# Paths
BUILD_DIR = "./build/Release"
RAYTRACE_EXE = os.path.join(BUILD_DIR, "raytrace.exe")
PPM_PATH = "static/im.ppm"
PNG_PATH = "static/im.png"
SCENE_JSON = "scene.json"

@app.route('/render', methods=['POST'])
def render_image():
    try:
        # Step 1: Get scene config from POST body
        scene_data = request.get_json(force=True)
        if not scene_data:
            return jsonify({'error': 'No JSON payload received'}), 400

        # Step 2: Write scene.json
        with open(SCENE_JSON, 'w') as f:
            json.dump(scene_data, f)

        # Step 3: Run raytracer
        result = subprocess.run([RAYTRACE_EXE], capture_output=True, text=True)
        if result.returncode != 0:
            return jsonify({'error': 'Raytracer failed', 'details': result.stderr}), 500

        # Step 4: Convert PPM to PNG
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
