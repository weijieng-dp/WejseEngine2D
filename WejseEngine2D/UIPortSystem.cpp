#include "ComponentFile.h"
#include "UIPortSystem.h"

static Registry& registry = Registry::instance();

bool collision(glm::vec3 circle1, glm::vec3 circle2, float circleRaius1, float circleRaius2)
{
	float distance1 = glm::distance(circle1, circle2);
	return distance1 <= circleRaius1 + circleRaius2;
}

void player(void)
{
	float dt = WejseGetDT();
	auto buffer = registry.getEntitiesWithComponent<UISelectComponent>();
	for (auto entity : buffer)
	{
		auto tmp = registry.getComponent<TransformComponent>(entity);
		auto tmpcom = registry.getComponent<UISelectComponent>(entity);
		if (tmp)
		{
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			{
				tmp->translate.x += 150 * dt;
			}
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			{
				tmp->translate.x -= 150 * dt;
			}
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			{
				tmp->translate.y += 150 * dt;
			}
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			{
				tmp->translate.y -= 150 * dt;
			}
		}
		
		if (tmpcom)
		{

			auto tmp1 = registry.getComponent<TransformComponent>(tmpcom->healthBar);
			auto tmp2 = registry.getComponent<TransformComponent>(tmpcom->damage);
			auto tmp3 = registry.getComponent<TransformComponent>(tmpcom->heal);
			bool flag{};
			static float maxHealth = tmp1->scale.x;

			if (tmp1 && tmp2 && tmp3)
			{
				
				if (collision(tmp2->translate, tmp->translate, tmp2->scale.x / 2, tmp->scale.x / 2) && tmp1->scale.x >= 0)
				{
					tmp1->scale.x -= 100 * dt;
				}

				if (collision(tmp3->translate, tmp->translate, tmp3->scale.x / 2, tmp->scale.x / 2) && tmp1->scale.x <= maxHealth)
				{
					tmp1->scale.x += 100 * dt;
				}
			}

			auto HealthBlock = registry.getComponent<TransformComponent>(tmpcom->healthblock);


			if (!tmpcom->spawned)
			{
				for (int i = 0; i < 10; i++)
				{
					auto ent = registry.createEntity();

					registry.addComponent<TransformComponent>(ent, TransformComponent(HealthBlock->translate + tmpcom->offset, 0, HealthBlock->scale));
					registry.addComponent<MeshRenderComponent>(ent, std::move(MeshRenderComponent("shader/shader.vs", "shader/shader.fs", glm::vec3(1, 0, 0))));

					tmpcom->offset += glm::vec3(100, 0, 0);

					tmpcom->healthBlock.push_back(ent);
				}
				tmpcom->spawned = true;
			}

			for (int i = 0; i < 10; i++)
			{
				auto activecomp = registry.getComponent<ActiveComponent>(tmpcom->healthBlock[i]);
				activecomp->isActive = true;

			}
			for (int j = 0; j < 10; j++)
			{
				/*if (tmp1->scale.x / 1000 * 100 <= 0)
				{
					auto activecomp = registry.getComponent<ActiveComponent>(tmpcom->healthBlock[0]);
					activecomp->isActive = false;
				}*/
				if (tmp1->scale.x / 1000 * 100 < ((j + 1.0) / 10) * 100)
				{
					//if (j == 0)
						//continue;
					auto activecomp = registry.getComponent<ActiveComponent>(tmpcom->healthBlock[j]);
					activecomp->isActive = false;

				}
			}

		}

		

	}

}