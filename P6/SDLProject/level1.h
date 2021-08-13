#include "Scene.h"
class Level1 : public Scene {
public:
	void Initialize(Entity* player) override;
	void Update(float deltaTime) override;
	void Render(ShaderProgram* program) override;
	void createEnemy(int enemyID);
};