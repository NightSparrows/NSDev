#pragma once


namespace ns {


	class EntityComponent {
		friend class Entity;
	public:
		virtual void init() {};

		virtual void cleanUp() {};
		
		virtual void update(float delta) {};

		virtual void render() {};

	protected:
		Entity *parent;
	};

}
