#include "StoreLoader.h"

StoreLoader::StoreLoader()
{
	m_sDatabaseDirectory = FileListing::getInstance()->getName("database_dir");

	m_sListSectionType = "type";
	m_sListSectionData = "data";
	m_sMapSectionType = "type";
	m_sMapSectionData = "data";
	m_sTableSectionDefs = "fdefs";
	m_sTableSectionRows = "rows";
}

StoreLoader::~StoreLoader()
{
}

bool StoreLoader::verifyStoreStructure(const std::string& sStoreName)
{
	std::string sStoreDir = m_sDatabaseDirectory + sStoreName + "\\";
	std::string sListsDir = m_sDatabaseDirectory + sStoreName + "\\lists\\";
	std::string sMapsDir = m_sDatabaseDirectory + sStoreName + "\\maps\\";
	std::string sTablesDir = m_sDatabaseDirectory + sStoreName + "\\tables\\";

	if (!m_fileUtils.directoryExists(sStoreDir))
	{
		if (!m_fileUtils.createDirectory(sStoreDir))
		{
			return false;
		}
	}
	if (!m_fileUtils.directoryExists(sListsDir))
	{
		if (!m_fileUtils.createDirectory(sListsDir))
		{
			return false;
		}
	}
	if (!m_fileUtils.directoryExists(sMapsDir))
	{
		if (!m_fileUtils.createDirectory(sMapsDir))
		{
			return false;
		}
	}
	if (!m_fileUtils.directoryExists(sTablesDir))
	{
		if (!m_fileUtils.createDirectory(sTablesDir))
		{
			return false;
		}
	}

	return true;
}

std::string StoreLoader::typeAsString(const DBSupportedTypes& type)
{
	switch (type)
	{
	case DBSupportedTypes::INT:
		return "int";
	case DBSupportedTypes::FLOAT:
		return "float";
	case DBSupportedTypes::STRING:
		return "string";
	default:
		return "int";
	}
}

DBSupportedTypes StoreLoader::typeFromString(const std::string& sString)
{
	if (sString == "string")
	{
		return DBSupportedTypes::STRING;
	}
	else if (sString == "float")
	{
		return DBSupportedTypes::FLOAT;
	}
	else
	{
		return DBSupportedTypes::INT;
	}
}

bool StoreLoader::loadList(const std::string& sListName, const std::string& sListsDir, Store& store)
{
	// Attempt to load the data
	NCFFile file(sListsDir, sListName);
	if (m_fileUtils.readNCFFile(file))
	{
		// Parse the header section first. The first piece of data is the supported type.
		// Discard any other data in this section.
		std::vector<std::string> data = file.getSection(m_sListSectionType);
		DBSupportedTypes type = DBSupportedTypes::INT;
		if (data.size() >= 1)
		{
			type = typeFromString(data.at(0));
		}

		// Create the list
		DBList list(type);

		// Parse the data section. Each piece of data should be converted to the target
		// type and then added to the list.
		data = file.getSection(m_sListSectionData);

		int i;
		float f;

		for (std::string sCurrentData : data)
		{
			switch (type)
			{
			case DBSupportedTypes::INT:
				i = m_utils.stringToInt(sCurrentData);
				list.add(&i);
				break;
			case DBSupportedTypes::FLOAT:
				f = m_utils.stringToFloat(sCurrentData);
				list.add(&f);
				break;
			case DBSupportedTypes::STRING:
				list.add(&sCurrentData);
				break;
			default:
				break;
			}
		}

		// Store it. Ensure extension is trimmed
		return store.addList(sListName.substr(0, sListName.find('.')), list);
	}
	else
	{
		return false;
	}
}

bool StoreLoader::saveList(const std::string& sListName, const std::string& sListsDir, DBList& list)
{
	// Create the NCFFile
	NCFFile file(sListsDir, sListName + ".txt");

	// Save the type
	DBSupportedTypes type = list.getType();
	file.addLineToSection(m_sListSectionType, typeAsString(type));

	// Save the data
	unsigned int uiCurrent = 0;
	unsigned int uiSize = list.size();
	for (; uiCurrent < uiSize; ++uiCurrent)
	{
		switch (type)
		{
		case DBSupportedTypes::STRING:
			file.addLineToSection(m_sListSectionData, *((std::string*)list.get(uiCurrent)));
			break;
		case DBSupportedTypes::INT:
			file.addLineToSection(m_sListSectionData, std::to_string(*((int*)list.get(uiCurrent))));
			break;
		case DBSupportedTypes::FLOAT:
			file.addLineToSection(m_sListSectionData, std::to_string(*((float*)list.get(uiCurrent))));
			break;
		default:
			break;
		}	
	}

	// Write to disk
	if (m_fileUtils.writeNCFFile(file))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool StoreLoader::loadMap(const std::string& sMapName, const std::string& sMapsDir, Store& store)
{
	// Attempt to load the data
	NCFFile file(sMapsDir, sMapName);
	if (m_fileUtils.readNCFFile(file))
	{
		// Parse the header section first. The first 2 pieces of data are the key and value
		// types. Discard all other data in this section.
		std::vector<std::string> data = file.getSection(m_sMapSectionType);
		DBSupportedTypes keyType = DBSupportedTypes::INT;
		DBSupportedTypes valType = DBSupportedTypes::INT;
		if (data.size() >= 2)
		{
			keyType = typeFromString(data.at(0));
			valType = typeFromString(data.at(1));
		}

		// Create the list
		DBMap map(keyType, valType);

		// Parse the data section. Each piece of data should be converted to the target
		// type and then added to the list.
		data = file.getSection(m_sMapSectionData);

		int iKey;
		float fKey;;
		int iVal;
		float fVal;

		for (std::string sCurrentData : data)
		{
			// Split and check there are two params
			std::vector<std::string> split = m_utils.splitString(sCurrentData, '|');
			if (split.size() < 2)
			{
				continue;
			}

			// Parse and add
			switch (keyType)
			{
			case DBSupportedTypes::INT:
				iKey = m_utils.stringToInt(split.at(0));
				switch (valType)
				{
				case DBSupportedTypes::INT:
					iVal = m_utils.stringToInt(split.at(1));
					map.add(&iKey, &iVal);
					break;
				case DBSupportedTypes::FLOAT:
					fVal = m_utils.stringToFloat(split.at(1));
					map.add(&iKey, &fVal);
					break;
				case DBSupportedTypes::STRING:
					map.add(&iKey, &split.at(1));
					break;
				default:
					break;
				}
				break;
			case DBSupportedTypes::FLOAT:
				fKey = m_utils.stringToFloat(split.at(0));
				switch (valType)
				{
				case DBSupportedTypes::INT:
					iVal = m_utils.stringToInt(split.at(1));
					map.add(&fKey, &iVal);
					break;
				case DBSupportedTypes::FLOAT:
					fVal = m_utils.stringToFloat(split.at(1));
					map.add(&fKey, &fVal);
					break;
				case DBSupportedTypes::STRING:
					map.add(&fKey, &split.at(1));
					break;
				default:
					break;
				}
				break;
			case DBSupportedTypes::STRING:
				switch (valType)
				{
				case DBSupportedTypes::INT:
					iVal = m_utils.stringToInt(split.at(1));
					map.add(&split.at(0), &iVal);
					break;
				case DBSupportedTypes::FLOAT:
					fVal = m_utils.stringToFloat(split.at(1));
					map.add(&split.at(0), &fVal);
					break;
				case DBSupportedTypes::STRING:
					map.add(&split.at(0), &split.at(1));
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}
		}

		// Store it. Ensure extension is trimmed
		return store.addMap(sMapName.substr(0, sMapName.find('.')), map);
	}
	else
	{
		return false;
	}
}

bool StoreLoader::saveMap(const std::string& sMapName, const std::string& sMapsDir, DBMap& map)
{
	// Create the NCFFile
	NCFFile file(sMapsDir, sMapName + ".txt");

	// Save the type
	DBSupportedTypes keyType = map.getKeyType();
	DBSupportedTypes valType = map.getValueType();
	file.addLineToSection(m_sMapSectionType, typeAsString(keyType));
	file.addLineToSection(m_sMapSectionType, typeAsString(valType));

	// Save the data
	unsigned int uiCurrent = 0;
	unsigned int uiSize = map.size();
	for (; uiCurrent < uiSize; ++uiCurrent)
	{
		std::string sData = "";

		std::pair<const void*, void*> data = map.get(uiCurrent);

		switch (keyType)
		{
		case DBSupportedTypes::STRING:
			sData += *((std::string*)data.first);
			break;
		case DBSupportedTypes::INT:
			sData += std::to_string(*((int*)data.first));
			break;
		case DBSupportedTypes::FLOAT:
			sData += std::to_string(*((float*)data.first));
			break;
		default:
			break;
		}

		sData += "|";

		switch (valType)
		{
		case DBSupportedTypes::STRING:
			sData += *((std::string*)data.second);
			break;
		case DBSupportedTypes::INT:
			sData += std::to_string(*((int*)data.second));
			break;
		case DBSupportedTypes::FLOAT:
			sData += std::to_string(*((float*)data.second));
			break;
		default:
			break;
		}

		file.addLineToSection(m_sMapSectionData, sData);
	}

	// Write to disk
	if (m_fileUtils.writeNCFFile(file))
	{
		return true;
	}
	else
	{
		return false;
	}
}

std::string StoreLoader::defAsString(const FieldDef& def)
{
	std::string s;

	s += def.sName;
	s += "|";
	s += typeAsString(def.type);
	s += "|";
	
	switch (def.type)
	{
	case DBSupportedTypes::INT:
		s += std::to_string(def.iDefaultIntValue);
		break;
	case DBSupportedTypes::FLOAT:
		s += std::to_string(def.fDefaultFloatValue);
		break;
	case DBSupportedTypes::STRING:
		s += def.sDefaultStringValue;
		break;
	}

	return s; 
}

FieldDef StoreLoader::defFromString(const std::string& sString)
{
	FieldDef def;

	std::vector<std::string> split = m_utils.splitString(sString, '|');
	if (split.size() == 2)
	{
		switch (typeFromString(split.at(1)))
		{
		case DBSupportedTypes::INT:
			def = FieldDef::createIntField(split.at(0));
			break;
		case DBSupportedTypes::FLOAT:
			def = FieldDef::createFloatField(split.at(0));
			break;
		case DBSupportedTypes::STRING:
			def = FieldDef::createStringField(split.at(0));
			break;
		}
	}
	else if (split.size() == 3)
	{
		switch (typeFromString(split.at(1)))
		{
		case DBSupportedTypes::INT:
			def = FieldDef::createIntField(split.at(0), m_utils.stringToInt(split.at(2)));
			break;
		case DBSupportedTypes::FLOAT:
			def = FieldDef::createFloatField(split.at(0), m_utils.stringToFloat(split.at(2)));
			break;
		case DBSupportedTypes::STRING:
			def = FieldDef::createStringField(split.at(0), split.at(2));
			break;
		}
	}

	return def;
}

bool StoreLoader::loadTable(const std::string& sTableName, const std::string& sTablesDir, Store& store)
{
	// Create the NCFFile and load the data
	NCFFile file(sTablesDir, sTableName);
	if (m_fileUtils.readNCFFile(file))
	{
		DBTable table;

		// Parse the field definitions first
		std::vector<std::string> data = file.getSection(m_sTableSectionDefs);
		std::vector<FieldDef> defs;
		for (std::string& sLine : data)
		{
			FieldDef def = defFromString(sLine);
			defs.push_back(def);
			table.createField(def);
		}

		// Parse the rows
		data = file.getSection(m_sTableSectionRows);
		for (std::string& sLine : data)
		{
			// The number of parameters should be one more than the number of field definitions
			// The order should also match
			std::vector<std::string> split = m_utils.splitString(sLine, '|');
			if (split.size() != (defs.size() + 1))
			{
				continue;
			}
			
			table.createRow(split.at(0));
			RowAccessor acc = table.getRowAccessor(split.at(0));
			split.erase(split.begin());

			unsigned int uiCurrent = 0;
			unsigned int uiSize = defs.size();
			for (; uiCurrent < uiSize; ++uiCurrent)
			{
				switch (defs.at(uiCurrent).type)
				{
				case DBSupportedTypes::INT:
					acc.setInt(defs.at(uiCurrent).sName, m_utils.stringToInt(split.at(uiCurrent)));
					break;
				case DBSupportedTypes::FLOAT:
					acc.setFloat(defs.at(uiCurrent).sName, m_utils.stringToFloat(split.at(uiCurrent)));
					break;
				case DBSupportedTypes::STRING:
					acc.setString(defs.at(uiCurrent).sName, split.at(uiCurrent));
					break;
				}
			}
		}

		// Store it. Ensure extension is trimmed
		return store.addTable(sTableName.substr(0, sTableName.find('.')), table);
	}
	else
	{
		return false;
	}
}

bool StoreLoader::saveTable(const std::string& sTableName, const std::string& sTablesDir, DBTable& table)
{
	// Create the NCFFile
	NCFFile file(sTablesDir, sTableName + ".txt");

	// Write the field definitions
	std::vector<FieldDef>& defs = table.getFieldDefs();
	for (const FieldDef& def : defs)
	{
		file.addLineToSection(m_sTableSectionDefs, defAsString(def));
	}

	// Write the rows
	std::map<std::string, Row>& rows = table.getRows();
	for (std::pair<const std::string, Row>& row : rows)
	{
		std::string sLine = "";
		std::map<std::string, int>& ints = row.second.getIntFields();
		std::map<std::string, float>& floats = row.second.getFloatFields();
		std::map<std::string, std::string>& strings = row.second.getStringFields();

		sLine += row.first + "|";

		for (const FieldDef& def : defs)
		{
			switch (def.type)
			{
			case DBSupportedTypes::INT:
				sLine += std::to_string(ints.at(def.sName));
				break;
			case DBSupportedTypes::FLOAT:
				sLine += std::to_string(floats.at(def.sName));
				break;
			case DBSupportedTypes::STRING:
				sLine += strings.at(def.sName);
				break;
			}

			sLine += "|";
		}

		file.addLineToSection(m_sTableSectionRows, sLine);
	}

	// Write to disk
	if (m_fileUtils.writeNCFFile(file))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool StoreLoader::loadStore(const std::string& sStoreName, Store& store)
{
	std::string sListsDir = m_sDatabaseDirectory + sStoreName + "\\lists\\";
	std::string sMapsDir = m_sDatabaseDirectory + sStoreName + "\\maps\\";
	std::string sTablesDir = m_sDatabaseDirectory + sStoreName + "\\tables\\";

	// Verify structure
	if (!verifyStoreStructure(sStoreName))
	{
		return false;
	}

	// Load lists
	std::vector<std::string> directories;
	if (!m_fileUtils.listFilesInDirectory(sListsDir, directories))
	{
	}
	else
	{	
		for (const std::string& sListName : directories)
		{
			loadList(sListName, sListsDir, store);
		}
	}	

	// Load maps
	directories.clear();
	if (!m_fileUtils.listFilesInDirectory(sMapsDir, directories))
	{
	}
	else
	{
		for (const std::string& sMapName : directories)
		{
			loadMap(sMapName, sMapsDir, store);
		}
	}

	// Load tables
	directories.clear();
	if (!m_fileUtils.listFilesInDirectory(sTablesDir, directories))
	{
	}
	else
	{
		for (const std::string& sTableName : directories)
		{
			loadTable(sTableName, sTablesDir, store);
		}
	}

	return true;
}

bool StoreLoader::saveStore(const std::string& sStoreName, Store& store)
{
	std::string sListsDir = m_sDatabaseDirectory + sStoreName + "\\lists\\";
	std::string sMapsDir = m_sDatabaseDirectory + sStoreName + "\\maps\\";
	std::string sTablesDir = m_sDatabaseDirectory + sStoreName + "\\tables\\";

	// Verify structure
	if (!verifyStoreStructure(sStoreName))
	{
		return false;
	}

	// Save lists
	std::map<std::string, DBList>& lists = store.getLists();
	for (std::pair<const std::string, DBList>& list : lists)
	{
		saveList(list.first, sListsDir, list.second);
	}

	// Save maps
	std::map<std::string, DBMap>& maps = store.getMaps();
	for (std::pair<const std::string, DBMap>& map : maps)
	{
		saveMap(map.first, sMapsDir, map.second);
	}

	// Save tables
	std::map<std::string, DBTable>& tables = store.getTables();
	for (std::pair<const std::string, DBTable>& table : tables)
	{
		saveTable(table.first, sTablesDir, table.second);
	}
	return true;
}