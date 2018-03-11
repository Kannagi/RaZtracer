#include <chrono>
#include <iostream>

#include "RaZtracer/RaZtracer.hpp"

int main() {
  Window window(800, 600, "RaZtracer");
  Scene scene;
  scene.enableAmbientOcclusion(true, 32);

  scene.addModel("../assets/meshes/cornell.obj");

  scene.setCamera(std::make_unique<Camera>(window.getWidth(), window.getHeight(), 45.f, 0.1f, 100.f, Vec3f({ 0.f, 0.f, 5.f })));

  const auto startTime = std::chrono::system_clock::now();
  ImagePtr img = scene.render();
  const auto endTime = std::chrono::system_clock::now();

  std::cout << "Scene rendering duration: "
            << std::chrono::duration_cast<std::chrono::duration<float>>(endTime - startTime).count()
            << " seconds." << std::endl;

  window.mapImage(std::move(img));

  while (window.show());

  return 0;
}
