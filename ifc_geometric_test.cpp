#include<iostream>
#include<chrono>
#define IfcSchema Ifc4

#include<ostream>
#include<regex>
#include<vector>
#include<Windows.h>
#include<fstream>
#include<string>
#include "../../IfcOpenshell/src/ifcparse/IfcFile.h"
#include "../../IfcOpenshell/src/ifcparse/Ifc4.h"

#if USE_VLD
#include <vld.h>
#endif
int main()
{
	SetConsoleOutputCP(65001);
	auto start = std::chrono::high_resolution_clock::now();

	std::string Ifcfile = "D:\\CimTestFile\\SZW_HRZB_MEP_2F.ifc";
	IfcParse::IfcFile file = Ifcfile;

	std::ofstream outputFile("geometric_test.txt");

	if (!outputFile.is_open()) {
		std::cout << "Unable to open output file" << std::endl;
		return 1;
	}
	Logger::SetOutput(&outputFile, &outputFile);

	if (!file.good()) {

		std::cout << "Unable to parse .ifc file" << std::endl;
		return 1;
	}

	
	
	
	//IfcSchema::IfcDuctSegment::list::ptr ductsegment = file.instances_by_type<IfcSchema::IfcDuctSegment>();
	//for (IfcSchema::IfcDuctSegment::list::it it = ductsegment->begin(); it != ductsegment->end(); ++it)
	//{
	//	const IfcSchema::IfcDuctSegment* master = *it;
	//	//std::vector<std::string>propertyVector;
	//	outputFile << master->data().toString() << std::endl;

	//	//ifcproertySet定义了所有extensible 属性，统一读取最后面的内容就可以了。
	//	::Ifc4::IfcRelDefinesByProperties::list::ptr RelProperty = master->IsDefinedBy();
	//	::Ifc4::IfcRelDefinesByType::list::ptr RelType = master->IsTypedBy();
	//	::Ifc4::IfcRelDefinesByObject::list::ptr RelObject = master->Declares();
	//	
	//	for (auto property : *RelProperty)
	//	{
	//		
	//		std::string content = property->RelatingPropertyDefinition()->data().toString();
	//		outputFile << property->RelatingPropertyDefinition()->data().toString() << std::endl;
	//		
	//	}
	//	}
	// 
	//IfcSchema::IfcShapeRepresentation::list::ptr tessellation = file.instances_by_type<IfcSchema::IfcShapeRepresentation>();
	//{
	//	for (IfcSchema::IfcShapeRepresentation::list::it it = tessellation->begin(); it != tessellation->end(); ++it)
	//	{
	//		const IfcSchema::IfcShapeRepresentation* master = *it;
	//		std::regex pattern(R"(^#(\d+)=\b)");
	//		std::smatch matches;
	//		std::string data_str = master->data().toString();
	//		if (std::regex_search(data_str, matches, pattern))
	//		{
	//			
	//			if (matches[1] == "180805")
	//			{
	//					查找句首的#[0-9]+用正则表达比较好。
	//				//todo获取到反属性。ifcreprentation来访问
	//				std::cout << "找到了：！！" << data_str << std::endl;
	//				::Ifc4::IfcRepresentationMap::list::ptr map = master->RepresentationMap();
	//				for (auto test : *map)
	//				{
	//					std::cout << test->data().toString() << std::endl;
	//				}
	//				/*std::cout << master->LayerAssignments();
	//				std::cout << master->RepresentationMap();
	//				std::cout << master->OfProductRepresentation();*/
	//				::Ifc4::IfcShapeAspect::list::ptr shape = master->OfShapeAspect();

	//				for (auto test : *shape)
	//				{
	//					std::cout << test->data().toString() << std::endl;
	//				}
	//				
	//			}		
	//		}
	//	}
	//}

	
	
	//记录所有terminal _info
	
	std::cout << "------------begin--------------" << std::endl;
	IfcSchema::IfcFlowTerminal::list::ptr IfcFlowTerminal_instance = file.instances_by_type< IfcSchema::IfcFlowTerminal>();
	for (IfcSchema::IfcFlowTerminal::list::it it = IfcFlowTerminal_instance->begin(); it != IfcFlowTerminal_instance->end(); ++it)
	{
		const IfcSchema::IfcFlowTerminal* master = *it;

		std::cout<< master->data().toString()<<std::endl;
		

		std:: string input = master->data().toString();
		std::istringstream iss(input);

		std::string token;
		int count=0;
		int IfcProductDefinitionShape_id =0;
		while (std::getline(iss, token, ','))
		{
			if (token.find('#') == 0)
			{
				++count;
				if (count == 4)
				{
					std::cout << "IfcProductDefinitionShape_id :	" << token << std::endl;	
					IfcProductDefinitionShape_id = std::stoi(token.substr(1));

					break;
				}
			}
		}
		
		IfcUtil::IfcBaseClass* IfcProductDefinitionShape_instance = file.instance_by_id(IfcProductDefinitionShape_id);
	
		std::string input1 = IfcProductDefinitionShape_instance->data().toString();
		
		std::cout << input1 << std::endl;
		
		
		std::regex pattern("#[0-9]+");
		std::sregex_iterator iter(input1.begin(), input1.end(), pattern);
		std::sregex_iterator end;
		std::vector<int>IfcShapeRepresentation_IdList;

		while (iter != end) {
			std::smatch match = *iter;
			std::string result = match.str().substr(1); // 去掉首部的 #
			IfcShapeRepresentation_IdList.push_back(std::stoi(result));
			++iter;
		}
		int IfcShapeRepresentation_id = IfcShapeRepresentation_IdList[1];
		std::cout << "IfcShapeRepresentation_id:	" << IfcShapeRepresentation_id << std::endl;
		
		

		//读取ifcMappedItem
		IfcUtil::IfcBaseClass* IfcShapeRepresentation_instance = file.instance_by_id(IfcShapeRepresentation_id);
		std::string input2 = IfcShapeRepresentation_instance->data().toString();
		std::cout << input2 << std::endl;

		std::sregex_iterator iter1(input2.begin(), input2.end(), pattern);
		std::sregex_iterator end1;
		std::vector<int>IfcMappedItem_IdList;

		while (iter1 != end1) {
			std::smatch match1 = *iter1;
			std::string result = match1.str().substr(1); // 去掉首部的 #
			IfcMappedItem_IdList.push_back(std::stoi(result));
			++iter1;
		}
		int IfcMappedItem_id = IfcMappedItem_IdList[2];
		std::cout << "IfcMappedItem_id:	" << IfcMappedItem_id << std::endl;
	

		//读取到IfcShapeRepresentation的几何内容，和它的反属性。
		IfcUtil::IfcBaseClass* IfcMappedItem_instance = file.instance_by_id(IfcMappedItem_id);
		std::cout << IfcMappedItem_instance->data().toString() << std::endl;
		std::string input3 = IfcMappedItem_instance->data().toString();
		std::sregex_iterator iter2(input3.begin(), input3.end(), pattern);
		std::sregex_iterator end2;
		std::vector<int>IfcRepresentationMap_IdList;
		while (iter2 != end2) {
			std::smatch match2 = *iter2;
			std::string result = match2.str().substr(1); // 去掉首部的 #
			IfcRepresentationMap_IdList.push_back(std::stoi(result));
			++iter2;
		}
		int IfcRepresentationMap_id = IfcRepresentationMap_IdList[1];
		std::cout << "ifcRepresentationMap_id:	" << IfcRepresentationMap_id << std::endl;
	
		
		//提取点坐标
		IfcUtil::IfcBaseClass* IfcRepresentationMap_instance = file.instance_by_id(IfcRepresentationMap_id);
		std::cout << IfcRepresentationMap_instance->data().toString() << std::endl;
		std::string input4 = IfcRepresentationMap_instance->data().toString();
		std::sregex_iterator iter3(input4.begin(), input4.end(), pattern);
		std::sregex_iterator end3;
		std::vector<int>GeoInfo_List;
		while (iter3 != end3) {
			std::smatch match3 = *iter3;
			std::string result = match3.str().substr(1); // 去掉首部的 #
			GeoInfo_List.push_back(std::stoi(result));
			++iter3;
		}
		
		//1后面全为点坐标
		for (int i = 2; i < GeoInfo_List.size(); i++)
		{
			std::cout << "IfcShapeRepresentation_id:	" << GeoInfo_List[2] << std::endl;
			IfcUtil::IfcBaseClass* IFcPoint_set = file.instance_by_id(GeoInfo_List[i]);
			std::cout << IFcPoint_set->data().toString() << std::endl;
			std::string input5 = IFcPoint_set->data().toString();
			std::sregex_iterator iter4(input5.begin(), input5.end(), pattern);
			std::sregex_iterator end4;
			std::vector<int>Point_instance;
			while (iter4 != end4) {
				std::smatch match4 = *iter4;
				std::string result = match4.str().substr(1); // 去掉首部的 #
				Point_instance.push_back(std::stoi(result));
				++iter4;
				
			}
			for (int j = 2; j < Point_instance.size(); j++)
			{
				IfcUtil::IfcBaseClass* point = file.instance_by_id(Point_instance[j]);
				std::cout << point->data().toString() << std::endl;
			}
			std::cout << "-------------------" << std::endl;
		
		}
		//读取点坐标
		

		////读取它的反属性
		//IfcSchema::IfcRepresentationMap ::list::ptr Map = file.instances_by_type<IfcSchema::IfcRepresentationMap>();
		//for (IfcSchema::IfcRepresentationMap::list::it it = Map->begin(); it != Map->end(); ++it)
		//{
		//	const IfcSchema::IfcRepresentationMap* master = *it;
		//	::Ifc4::IfcRepresentation test=*master->MappedRepresentation();
		//	::Ifc4::IfcShapeAspect::list::ptr test2=master->HasShapeAspects();
		//	std::cout << &test << std::endl;

		//	for (auto property : *test2)
		//	{
		//		std::cout << property->data().toString() << std::endl;
		//	}
		//	std::cout << "------------手动分割---------------" << std::endl;

		//}
	

	}
	outputFile.close();
	




	


	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = end - start;
	std::cout << "Code execution time: " << duration.count() << " seconds" << std::endl;
	return 0;





	

}