#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(25);
	ofSetWindowTitle("openFrameworks");

	ofBackground(39);
	ofNoFill();
	ofEnableDepthTest();

	this->line.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}
//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->face.clear();
	this->line.clear();

	for (int radius = 0; radius <= 1000; radius += 10) {

		int start_index = this->line.getNumVertices();
		int index = start_index;
		for (int deg = 0; deg < 360; deg += 1) {

			auto z = radius > 200 ? 0 : ofMap(radius, 0, 200, -1 * pow(200 - radius, 1.1), 0);

			auto vertex = glm::vec3(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD), z + 1);
			this->line.addVertex(vertex);

			if (deg > 0) {

				this->line.addIndex(index - 1);
				this->line.addIndex(index);
			}

			index++;

			int next_radius = radius + 10;
			int next_z = next_radius > 200 ? 0 : ofMap(next_radius, 0, 200, -1 * pow(200 - next_radius, 1.1), 0);
			vector<glm::vec3> vertices;
			vertices.push_back(glm::vec3(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD), z));
			vertices.push_back(glm::vec3(radius * cos((deg + 1) * DEG_TO_RAD), radius * sin((deg + 1) * DEG_TO_RAD), z));
			vertices.push_back(glm::vec3(next_radius * cos((deg + 1) * DEG_TO_RAD), next_radius * sin((deg + 1) * DEG_TO_RAD), next_z));
			vertices.push_back(glm::vec3(next_radius * cos(deg * DEG_TO_RAD), next_radius * sin(deg * DEG_TO_RAD), next_z));

			face.addVertices(vertices);

			face.addIndex(face.getNumVertices() - 1); face.addIndex(face.getNumVertices() - 2); face.addIndex(face.getNumVertices() - 3);
			face.addIndex(face.getNumVertices() - 1); face.addIndex(face.getNumVertices() - 3); face.addIndex(face.getNumVertices() - 4);

		}

		this->line.addIndex(this->line.getNumVertices() - 1);
		this->line.addIndex(start_index);
	}

	int index_span = 15;
	for (int index = 0; index < this->line.getNumVertices() - 360; index += index_span) {

		this->line.addIndex(index);
		this->line.addIndex(index + 360);
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateX(-70);

	ofSetColor(39);
	this->face.draw();

	ofColor color;
	for (int i = 0; i < 500; i++) {

		auto radius_base = ofRandom(1000);
		auto speed = ofRandom(8, 16);
		int deg = ofRandom(360);
		int len = ofRandom(10, 20);

		int tmp_radius = (int)(radius_base + ofGetFrameNum() * speed) % 1000;
		auto alpha = tmp_radius < 300 ? 255 : ofMap(tmp_radius, 300, 1000, 255, 0);

		color.setHsb(ofMap(i, 0, 500, 150, 220), 150, 255);
		ofSetColor(color, alpha);

		ofSetLineWidth(ofRandom(0.5, 2));

		ofBeginShape();

		for (int k = 0; k < len; k++) {

			int radius = (int)(radius_base + (ofGetFrameNum() + k) * speed) % 1000;
			auto z = radius > 200 ? 1 : ofMap(radius, 0, 200, -1 * pow(200 - radius, 1.1), 1);

			auto vertex = glm::vec3(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD), z + 1);
			ofVertex(vertex);
		}

		ofEndShape();
	}

	this->cam.end();

	/*
	int start = 50;
	if (ofGetFrameNum() > start) {

		ostringstream os;
		os << setw(4) << setfill('0') << ofGetFrameNum() - start;
		ofImage image;
		image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		image.saveImage("image/cap/img_" + os.str() + ".jpg");
		if (ofGetFrameNum() - start >= 25 * 20) {

			std::exit(1);
		}
	}
	*/
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}