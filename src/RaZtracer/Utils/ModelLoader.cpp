#include <algorithm>
#include <fstream>
#include <sstream>
#include <unordered_map>

#include "RaZtracer/Render/Mesh.hpp"
#include "RaZtracer/Utils/ModelLoader.hpp"
#include "RaZtracer/Utils/MtlLoader.hpp"

namespace {

const std::string extractFileExt(const std::string& fileName) {
  return (fileName.substr(fileName.find_last_of('.') + 1));
}

void importObj(std::ifstream& file, std::vector<DrawableShapePtr>& shapes) {
  MeshPtr mesh = std::make_unique<Mesh>();
  std::unordered_map<std::string, std::size_t> materialCorrespIndices;

  std::vector<Vec3f> positions;
  std::vector<Vec2f> texcoords;
  std::vector<Vec3f> normals;

  std::vector<std::vector<int64_t>> posIndices(1);
  std::vector<std::vector<int64_t>> texcoordsIndices(1);
  std::vector<std::vector<int64_t>> normalsIndices(1);

  while (!file.eof()) {
    std::string line;
    file >> line;

    if (line[0] == 'v') {
      if (line[1] == 'n') { // Normals
        Vec3f normalsTriplet {};

        file >> normalsTriplet[0]
             >> normalsTriplet[1]
             >> normalsTriplet[2];

        normals.push_back(normalsTriplet);
      } else if (line[1] == 't') { // Texcoords
        Vec2f texcoordsTriplet {};

        file >> texcoordsTriplet[0]
             >> texcoordsTriplet[1];

        texcoords.push_back(texcoordsTriplet);
      } else { // Vertices
        Vec3f positionTriplet {};

        file >> positionTriplet[0]
             >> positionTriplet[1]
             >> positionTriplet[2];

        positions.push_back(positionTriplet);
      }
    } else if (line[0] == 'f') { // Faces
      std::getline(file, line);

      const char delim = '/';
      const auto nbVertices = static_cast<uint16_t>(std::count(line.cbegin(), line.cend(), ' '));
      const auto nbParts = static_cast<uint8_t>(std::count(line.cbegin(), line.cend(), delim) / nbVertices + 1);
      const bool quadFaces = (nbVertices == 4);

      std::stringstream indicesStream(line);
      std::vector<int64_t> partIndices(nbParts * nbVertices);
      std::string vertex;

      for (std::size_t vertIndex = 0; vertIndex < nbVertices; ++vertIndex) {
        indicesStream >> vertex;

        std::stringstream vertParts(vertex);
        std::string part;
        uint8_t partIndex = 0;

        while (std::getline(vertParts, part, delim)) {
          if (!part.empty())
            partIndices[partIndex * nbParts + vertIndex + (partIndex * quadFaces)] = std::stol(part);

          ++partIndex;
        }
      }

      if (quadFaces) {
        posIndices.back().emplace_back(partIndices[0]);
        posIndices.back().emplace_back(partIndices[2]);
        posIndices.back().emplace_back(partIndices[3]);

        texcoordsIndices.back().emplace_back(partIndices[4]);
        texcoordsIndices.back().emplace_back(partIndices[6]);
        texcoordsIndices.back().emplace_back(partIndices[7]);

        normalsIndices.back().emplace_back(partIndices[8]);
        normalsIndices.back().emplace_back(partIndices[10]);
        normalsIndices.back().emplace_back(partIndices[11]);
      }

      posIndices.back().emplace_back(partIndices[0]);
      posIndices.back().emplace_back(partIndices[1]);
      posIndices.back().emplace_back(partIndices[2]);

      texcoordsIndices.back().emplace_back(partIndices[3 + quadFaces]);
      texcoordsIndices.back().emplace_back(partIndices[4 + quadFaces]);
      texcoordsIndices.back().emplace_back(partIndices[5 + quadFaces]);

      const auto quadStride = static_cast<uint8_t>(quadFaces * 2);

      normalsIndices.back().emplace_back(partIndices[6 + quadStride]);
      normalsIndices.back().emplace_back(partIndices[7 + quadStride]);
      normalsIndices.back().emplace_back(partIndices[8 + quadStride]);
    } else if (line[0] == 'm') {
      std::string materialName;
      file >> materialName;

      MtlLoader::importMtl(materialName, mesh->getMaterials(), materialCorrespIndices);
    } else if (line[0] == 'u') {
      std::string materialName;
      file >> materialName;

      mesh->getSubmeshes().back()->setMaterialIndex(materialCorrespIndices.find(materialName)->second);
    } else if (line[0] == 'o' || line[0] == 'g') {
      if (!posIndices.front().empty()) {
        const std::size_t newSize = posIndices.size() + 1;
        posIndices.resize(newSize);
        texcoordsIndices.resize(newSize);
        normalsIndices.resize(newSize);

        mesh->addSubmesh(std::make_unique<Submesh>());
      }

      std::getline(file, line);
    } else {
      std::getline(file, line); // Skip the rest of the line
    }
  }

  for (std::size_t submeshIndex = 0; submeshIndex < mesh->getSubmeshes().size(); ++submeshIndex) {
    for (std::size_t partIndex = 0; partIndex < posIndices[submeshIndex].size(); partIndex += 3) {
      const std::size_t secondPartIndex = partIndex + 1;
      const std::size_t thirdPartIndex = partIndex + 2;

      int64_t tempPosIndex = posIndices[submeshIndex][partIndex];
      const std::size_t firstPosIndex = (tempPosIndex < 0 ? tempPosIndex + positions.size() : tempPosIndex - 1ul);
      tempPosIndex = posIndices[submeshIndex][secondPartIndex];
      const std::size_t secondPosIndex = (tempPosIndex < 0 ? tempPosIndex + positions.size() : tempPosIndex - 1ul);
      tempPosIndex = posIndices[submeshIndex][thirdPartIndex];
      const std::size_t thirdPosIndex = (tempPosIndex < 0 ? tempPosIndex + positions.size() : tempPosIndex - 1ul);

      shapes.emplace_back(std::make_unique<Triangle>(positions[firstPosIndex],
                                                     positions[secondPosIndex],
                                                     positions[thirdPosIndex]));
      if (!mesh->getMaterials().empty())
        shapes.back()->setMaterial(mesh->getMaterials()[mesh->getSubmeshes()[submeshIndex]->getMaterialIndex()]);
    }
  }
}

void importOff(std::ifstream& file, std::vector<DrawableShapePtr>& shapes) {
  std::size_t vertexCount, faceCount;
  file.ignore(3);
  file >> vertexCount >> faceCount;
  file.ignore(100, '\n');

  std::vector<Vec3f> positions(vertexCount);

  for (std::size_t vertexIndex = 0; vertexIndex < vertexCount; ++vertexIndex)
    file >> positions[vertexIndex][0] >> positions[vertexIndex][1] >> positions[vertexIndex][2];

  for (std::size_t faceIndex = 0; faceIndex < faceCount; ++faceIndex) {
    uint16_t partCount {};
    std::size_t firstIndex {}, secondIndex {}, thirdIndex {};

    file >> partCount;
    file >> firstIndex >> secondIndex >> thirdIndex;

    shapes.emplace_back(std::make_unique<Triangle>(positions[firstIndex], positions[secondIndex], positions[thirdIndex]));

    if (partCount >= 4) {
      std::size_t fourthIndex {};
      file >> fourthIndex;

      shapes.emplace_back(std::make_unique<Triangle>(positions[firstIndex], positions[thirdIndex], positions[fourthIndex]));
    }

    file.ignore(100, '\n');
  }
}

} // namespace

void ModelLoader::importModel(const std::string& fileName, std::vector<DrawableShapePtr>& shapes) {
  std::ifstream file(fileName, std::ios_base::in | std::ios_base::binary);

  if (file) {
    const std::string format = extractFileExt(fileName);

    if (format == "obj" || format == "OBJ")
      importObj(file, shapes);
    else if (format == "off" || format == "OFF")
      importOff(file, shapes);
    else
      throw std::runtime_error("Error: '" + format + "' format is not supported");
  } else {
    throw std::runtime_error("Error: Couldn't open the file '" + fileName + "'");
  }
}
