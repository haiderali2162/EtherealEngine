#include "Utils.h"
#include "core/serialization/serialization.h"
#include "core/serialization/archives.h"
#include "../Meta/Ecs/Entity.hpp"

namespace ecs
{
	namespace utils
	{
		void saveEntity(const fs::path& dir, const core::Entity& data)
		{
			const fs::path fullPath = fs::resolve_protocol(dir / fs::path(data.get_name()+ ".asset"));
			saveData(fullPath, { data });
		}

		bool tryLoadEntity(const fs::path& fullPath, core::Entity& outData)
		{
			std::vector<core::Entity> outDataVec;
			if (!loadData(fullPath, outDataVec))
				return false;

			if (!outDataVec.empty())
				outData = outDataVec[0];

			return true;
		}


		void saveData(const fs::path& fullPath, const std::vector<core::Entity>& data)
		{
			std::ofstream os(fullPath, std::fstream::binary | std::fstream::trunc);
			serializeData(os, data);
		}

		bool loadData(const fs::path& fullPath, std::vector<core::Entity>& outData)
		{
			std::ifstream is(fullPath, std::fstream::binary);
			return deserializeData(is, outData);
		}

		void serializeData(std::ostream& stream, const std::vector<core::Entity>& data)
		{
			cereal::OArchive_Binary ar(stream);

			ar(
				cereal::make_nvp("data", data)
			);
			getSerializationMap().clear();
		}

		bool deserializeData(std::istream& stream, std::vector<core::Entity>& outData)
		{
			// get length of file:
			getSerializationMap().clear();
			stream.seekg(0, stream.end);
			std::streampos length = stream.tellg();
			stream.seekg(0, stream.beg);
			if (length > 0)
			{
				cereal::IArchive_Binary ar(stream);

				ar(
					cereal::make_nvp("data", outData)
				);

				stream.clear();
				stream.seekg(0);
				getSerializationMap().clear();
				return true;
			}
			return false;
		}
	}
}
