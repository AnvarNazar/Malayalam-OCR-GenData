#ifndef LETTERS
#define LETTERS
class Letters
{
public:
    std::map<std::string, int> mLetters;
    std::string fileName;
    void readLetters();
    Letters(char *fileName);
    ~Letters();
};
#endif // LETTERS_H
