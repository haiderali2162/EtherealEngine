#include "model.h"
#include "material.h"
#include "mesh.h"
#include "core/math/math_includes.h"
#include "../assets/asset_manager.h"


Model::Model()
{
	auto am = core::get_subsystem<AssetManager>();
	am->load<Material>("engine_data://materials/standard", false)
		.then([this](auto asset)
	{
		mMaterials.push_back(asset);
	});
}

bool Model::isValid() const
{
	return !mMeshLods.empty();
}

AssetHandle<Mesh> Model::getLod(std::uint32_t lod) const
{
	if (mMeshLods.size() > lod)
	{
		auto lodMesh = mMeshLods[lod];
		if (lodMesh)
			return lodMesh;

		for (unsigned int i = lod; i < mMeshLods.size(); ++i)
		{
			auto lodMesh = mMeshLods[i];
			if (lodMesh)
				return lodMesh;
		}
		for (unsigned int i = lod; i > 0; --i)
		{
			auto lodMesh = mMeshLods[i];
			if (lodMesh)
				return lodMesh;
		}
	}
	return AssetHandle<Mesh>();
}

void Model::setLod(AssetHandle<Mesh> mesh, std::uint32_t lod)
{
	if (lod >= mMeshLods.size())
		mMeshLods.resize(lod + 1);

	mMeshLods[lod] = mesh;
}

void Model::setMaterial(AssetHandle<Material> material, std::uint32_t index)
{
	if (index >= mMeshLods.size())
		mMeshLods.resize(index + 1);

	mMaterials[index] = material;
}

const std::vector<AssetHandle<Mesh>>& Model::getLods() const
{
	return mMeshLods;
}

void Model::setLods(const std::vector<AssetHandle<Mesh>>& lods)
{
	mMeshLods = lods;
}


const std::vector<AssetHandle<Material>>& Model::getMaterials() const
{
	return mMaterials;
}

void Model::setMaterials(const std::vector<AssetHandle<Material>>& materials)
{
	mMaterials = materials;
}


AssetHandle<Material> Model::getMaterialForGroup(const Group& group) const
{
	// TODO implement this
	if (mMaterials.empty())
		return AssetHandle<Material>();

	return mMaterials[0];
}

void Model::setMaxDistance(float distance)
{
	if (distance < mMinDistance)
		distance = mMinDistance;

	mMaxDistance = distance;
}

void Model::setMinDistance(float distance)
{
	if (distance > mMaxDistance)
		distance = mMaxDistance;

	mMinDistance = distance;
}
