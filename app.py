from flask import Flask, send_file, jsonify
import subprocess
import os

app = Flask(__name__)

# Absolute path to raytrace.exe inside build/Release/
RAYTRACE_EXE = os.path.join(os.getcwd(), 'build', 'raytrace')

# Path to output image
IMAGE_PATH = os.path.join(os.getcwd(), 'static', 'im.ppm')

@app.route('/render')
def render_image():
    try:
        # Run the raytracer executable
        result = subprocess.run([RAYTRACE_EXE], capture_output=True, text=True)

        if result.returncode != 0:
            return jsonify({'error': 'Raytracer failed', 'details': result.stderr}), 500

        return jsonify({'message': 'Image rendered successfully'})
    except Exception as e:
        return jsonify({'error': str(e)}), 500


@app.route('/image')
def get_image():
    # Serve the image if it exists
    if os.path.exists(IMAGE_PATH):
        return send_file(IMAGE_PATH, mimetype='image/x-portable-pixmap')
    else:
        return jsonify({'error': 'Image not found'}), 404


if __name__ == '__main__':
    app.run(debug=True)
