#include "FBXDocument.h"

Arc::FBXDocument Arc::FBXDocument::INVALID = FBXDocument();

Arc::FBXType Arc::FBXDocument::FBX_TYPE_ASCII = "FBXType.ascii";

Arc::FBXType Arc::FBXDocument::FBX_TYPE_BINARY = "FBXType.binary";

void Arc::FBXDocument::loadFile( const string& filename, const FBXType& type )
{
    Buffer buff = Buffer::LoadFromFile(filename);
	loadBuffer(buff, type);
}

void Arc::FBXDocument::loadString( const string& data, const FBXType& type )
{
    Buffer buff = Buffer(data);
	loadBuffer(buff, type);
}

void Arc::FBXDocument::loadBuffer( Buffer& data, const FBXType& type )
{
	if (type == FBX_TYPE_ASCII)
	{
		loadBufferASCII(data);
	}
	else if (type == FBX_TYPE_BINARY)
	{
		loadBufferBinary(data);
	}
}

void Arc::FBXDocument::loadBufferASCII( Buffer& data )
{
	data.resetReadIndex();

	string line;
	string objName = "";
	string objNameExt = "";

	bool multiLine = false;

	ArrayList<string> path;
	ArrayList<string> pathExt;

	string key = "";
	string val = "";

	string modelID = "";
	string modelName = "";
	ArrayList<float> modelVerts;

	while ( ! data.endOfBuffer())
	{
		line = data.readNextLine();

		if (line.length() == 0)
			continue;

		if (line[0] == ';')
			continue;

		if (Arc_StringContains(line, "{"))
		{
			ArrayList<string> objNameParts = Arc_StringSplit(line, ':', 2);

			if (objNameParts.getSize() == 2)
			{
				objName = objNameParts[0];
				Arc_Trim(objName);
				path.add(objName);

				ArrayList<string> objNameExtParts = Arc_StringSplit(objNameParts[1], '{', 2);

				if (objNameExtParts.getSize() >= 1)
				{
					objNameExt = objNameExtParts[0];
					Arc_Trim(objNameExt);
					pathExt.add(objNameExt);
				}
				else
				{
					pathExt.add("");
				}
			}
			else
			{
				path.add("");
				pathExt.add("");
			}


		}

		if (Arc_StringContains(line, "}"))
		{
			path.popBack();
			pathExt.popBack();

			objName = (path.isEmpty() ? "" : path.getBack());
			objNameExt = (pathExt.isEmpty() ? "" : pathExt.getBack());

			continue;
		}

		if (multiLine)
		{
			val = Arc_GetTrim(line);
		}
		else
		{
			ArrayList<string> splitLine = Arc_StringSplit(line, ':', 2);

			if (splitLine.getSize() != 2)
				continue;

			key = splitLine[0];
			val = splitLine[1];

			Arc_Trim(key);
			Arc_Trim(val);
		}

		if (path.isEmpty())
		{
			StripQuotes(val);

			if (key == "CreationTime")
			{
				setCreationTime(val);
			}
			else if (key == "Creator")
			{
				setCreator(val);
			}
		}
		else
		{
			if (path.contains("FBXHeaderExtension") && key != "FBXHeaderExtension")
			{
				StripQuotes(val);

				if (path.contains("CreationTimeStamp") && key != "CreationTimeStamp")
				{
					int intVal = Arc_ParseInt(val);

					if (key == "Version")
					{
						getHeader().getTimeStamp().setVersion(intVal);
					}
					else if (key == "Year")
					{
						getHeader().getTimeStamp().setYear(intVal);
					}
					else if (key == "Month")
					{
						getHeader().getTimeStamp().setMonth(intVal);
					}
					else if (key == "Day")
					{
						getHeader().getTimeStamp().setDay(intVal);
					}
					else if (key == "Hour")
					{
						getHeader().getTimeStamp().setHours(intVal);
					}
					else if (key == "Minute")
					{
						getHeader().getTimeStamp().setMinutes(intVal);
					}
					else if (key == "Second")
					{
						getHeader().getTimeStamp().setSeconds(intVal);
					}
					else if (key == "Millisecond")
					{
						getHeader().getTimeStamp().setMilliseconds(intVal);
					}
				}
				if (path.contains("OtherFlags") && key != "OtherFlags")
				{
					getHeader().setFlag(key, Arc_ParseBool(val));
				}
				else if (key == "FBXHeaderVersion")
				{
					getHeader().setHeaderVersion(Arc_ParseInt(val));
				}
				else if (key == "FBXVersion")
				{
					getHeader().setVersion(Arc_ParseInt(val));
				}
				else if (key == "Creator")
				{
					getHeader().setCreator(val);
				}
			}
			else if (path.contains("Definitions") && key != "Definitions")
			{
				int intVal = Arc_ParseInt(val);

				if (path.contains("ObjectType") && key != "ObjectType")
				{
					string typeName = objNameExt;
					StripQuotes(typeName);
					getDefinitions().setTypeCount(typeName, intVal);
				}
				else if (key == "Version")
				{
					getDefinitions().setVersion(intVal);
				}
				else if (key == "Count")
				{
					// Ignore
				}
			}
			else if (path.contains("Objects") && key != "Objects")
			{
				if (path.contains("Model"))
				{
					if (key == "Model")
					{
						ArrayList<string> extNameParts = Arc_StringSplit(objNameExt, ',');

						if (extNameParts.getSize() != 2)
							continue;

						modelVerts.clear();

						modelName = extNameParts[0];
						modelID = extNameParts[1];

						Arc_Trim(modelName);
						Arc_Trim(modelID);

						StripQuotes(modelName);
						StripQuotes(modelID);

						getObjects().addModel(modelID);

						FBXModel& model = getObjects().getModel(modelID);

						if (model.getID() == "")
							continue;

						model.setName(modelName);
					}
					else
					{
						FBXModel& model = getObjects().getModel(modelID);

						if (model.getID() == "")
							continue;

						if (key == "Properties60")
						{

						}
						else if (key == "Properties70")
						{

						}
						else if (key == "Vertices")
						{
							ArrayList<string> verts = Arc_StringSplit(val, ',');

							if (verts.getSize() < 3)
								continue;

							for (unsigned int i = 0; i < verts.getSize(); ++i)
							{
								modelVerts.add(Arc_ParseFloat(verts[i]));
							}

							if (val.back() == ',')
							{
								multiLine = true;
							}
							else
							{
								multiLine = false;

								for (unsigned int i = 0; i < modelVerts.getSize(); i += 3)
								{
									if (i + 3 > modelVerts.getSize())
										break;

									model.addVertex(Vector3(modelVerts[i], modelVerts[i + 1], modelVerts[i + 2]));
								}
							}
						}
						else if (key == "PolygonVertexIndex")
						{
							ArrayList<string> parts = Arc_StringSplit(val, ',');
							ArrayList<int> inds;

							for (unsigned int i = 0; i < parts.getSize(); ++i)
							{
								if (parts.getSize() == 0)
									continue;

								int ind = Arc_ParseInt(parts[i]);
								if (Arc_Sign(ind) == -1)
									ind = abs(ind) - 1;
								inds.add(ind);

								if (parts[i][0] == '-')
								{
									model.addFace(inds);
									inds.clear();
								}
							}
						}
					}
				}
			}
			else if (path.contains("Connections"))
			{

			}
		}
	}
}

void Arc::FBXDocument::loadBufferBinary( Buffer& data )
{
	
}

void Arc::FBXDocument::StripQuotes( string& str )
{
	if (str.length() >= 2 && str[0] == '"' && str[str.length() - 1] == '"')
	{
		str = Arc_Substring(str, 1, str.length() - 2);
	}
}
