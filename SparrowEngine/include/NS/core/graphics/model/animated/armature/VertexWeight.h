#pragma once

namespace ns {

	class VertexWeight {

	private:
		unsigned int jointId;
		unsigned int vertexId;
		float weight;

	public:
		VertexWeight(unsigned int jointId, unsigned int vertexId, float weight) {
			this->jointId = jointId;
			this->vertexId = vertexId;
			this->weight = weight;
		}

		unsigned int getBoneId() {
			return this->jointId;
		}

		unsigned int getVertexId() {
			return this->vertexId;
		}

		float getWeight() {
			return this->weight;
		}
	};

}