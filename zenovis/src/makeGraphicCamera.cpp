#include <zeno/utils/vec.h>
#include <zenovis/Camera.h>
#include <zenovis/IGraphic.h>
#include <zenovis/Scene.h>
#include <zenovis/ShaderManager.h>
#include <zenovis/makeGraphic.h>
#include <zenovis/opengl/buffer.h>
#include <zenovis/opengl/shader.h>
#include <zeno/types/CameraObject.h>

namespace zenovis {
namespace {

using opengl::Buffer;
using opengl::Program;
using zeno::vec3f;

struct GraphicCamera : IGraphic {
    Scene *scene;

    std::unique_ptr<Buffer> vbo;
    size_t vertex_count;

    Program *prog;

    explicit GraphicCamera(Scene *scene_, std::shared_ptr<zeno::CameraObject> cam) : scene(scene_) {
        scene->camera->setCamera(
                glm::vec3(cam->pos[0], cam->pos[1], cam->pos[2]),
                glm::vec3(cam->view[0], cam->view[1], cam->view[2]),
                glm::vec3(cam->up[0], cam->up[1], cam->up[2]),
                cam->fov, cam->fnear, cam->ffar, cam->dof);
    }

    virtual void draw(bool reflect, float depthPass) override {
    }

    virtual void drawShadow(Light *light) override {
    }
};

}

std::unique_ptr<IGraphic> makeGraphicCamera(Scene *scene, std::shared_ptr<zeno::IObject> obj) {
    if (auto cam = std::dynamic_pointer_cast<zeno::CameraObject>(obj))
        return std::make_unique<GraphicCamera>(scene, std::move(cam));
    return nullptr;
}

}