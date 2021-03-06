#include <Arc/TestGroup.h>

#include <Arc/OBJDocument.h>

using namespace Arc;

TestResult Test_ArcData_OBJDocument_SimpleCube( void )
{
	const string cube =
		"# This is a test comment\n"
		"v 1.000000 -1.000000 -1.000000\n"
		"v 1.000000 -1.000000 1.000000\n"
		"v -1.000000 -1.000000 1.000000\n"
		"v -1.000000 -1.000000 -1.000000\n"
		"v 1.000000 1.000000 -1.000000\n"
		"v 1.000000 1.000000 1.000000\n"
		"v -1.000000 1.000000 1.000000\n"
		"v -1.000000 1.000000 -1.000000\n"
		"f 1 2 3 4\n"
		"f 5 8 7 6\n"
		"f 1 5 6 2\n"
		"f 2 6 7 3\n"
		"f 3 7 8 4\n"
		"f 5 1 4 8\n";

	const unsigned int NUM_VERTICES = 8;
	const unsigned int NUM_FACES = 6;

	const Vector3 vertices[NUM_VERTICES] = {
		Vector3( 1.0f, -1.0f, -1.0f),
		Vector3( 1.0f, -1.0f,  1.0f),
		Vector3(-1.0f, -1.0f,  1.0f),
		Vector3(-1.0f, -1.0f, -1.0f),
		Vector3( 1.0f,  1.0f, -1.0f),
		Vector3( 1.0f,  1.0f,  1.0f),
		Vector3(-1.0f,  1.0f,  1.0f),
		Vector3(-1.0f,  1.0f, -1.0f),
	};

	const int faces[NUM_FACES][4] = {
		{ 0, 1, 2, 3 },
		{ 4, 7, 6, 5 },
		{ 0, 4, 5, 1 },
		{ 1, 5, 6, 2 },
		{ 2, 6, 7, 3 },
		{ 4, 0, 3, 7 },
	};

	OBJDocument doc;
	doc.loadString(cube);

	if (doc.getNumVertices() != NUM_VERTICES)
		return TestGroup::Failure("Invalid number of vertices");

	if (doc.getNumFaces() != NUM_FACES)
		return TestGroup::Failure("Invalid number of faces");

	bool correctVerts = true;
	bool correctFaces = true;

	for (unsigned int i = 0; i < NUM_VERTICES; ++i)
	{
		if (vertices[i] != doc.getVertex(i))
		{
			correctVerts = false;
			break;
		}
	}

	if ( ! correctVerts)
		return TestGroup::Failure("Invalid vertex data");

	for (unsigned int i = 0; i < NUM_FACES; ++i)
	{
		const OBJFace& face = doc.getFace(i);

		if (face.getNumVertices() != 4)
		{
			correctFaces = false;
			break;
		}

		for (unsigned int j = 0; j < 4; ++j)
		{
			if (face.getVertex(j) != vertices[faces[i][j]])
			{
				correctFaces = false;
				break;
			}
		}
	}

	if ( ! correctFaces)
		return TestGroup::Failure("Invalid face data");

	return TestGroup::Success();
}

void TestGroup_ArcData_OBJDocument( TestGroup& test )
{
	test.addTest("ArcData_OBJDocument_SimpleCube", Test_ArcData_OBJDocument_SimpleCube);
}