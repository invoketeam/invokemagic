#pragma warning (disable : 4786)



//example:  testSave(test.ttf, export.xml, export.png,  64,512,0)
//edge is needed when you want to add outlines later in photoshop or something
//addspace -- because the outline the letters will be bigger, add some extra spacing
extern void saveFont(std::string testName, std::string fontFile, std::string fontSkin, int psize=64, int endchar=128, int edge=0, int addspace=0);
