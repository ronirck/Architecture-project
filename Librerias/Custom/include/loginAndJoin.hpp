#include "./headers.hpp"
#ifndef LOGINANDJOIN_H
#define LOGINANDJOIN_H
using json = nlohmann::json;
using string = std::string;
void loadEnvFile(const string &filename);
string getEnvVar(const string &key);
string encryptAES(const string &plainText, const string &key, const string &iv);
string decryptAES(const string &cipherText, const string &key, const string &iv);
unsigned long generateUserId(const string &name);
void saveEncryptedJson(const json &data, const string &key, const string &iv);
json loadEncryptedJson(const string &key, const string &iv);
void createUser(const string &name, const string &lastName, const string &password, const string &key, const string &iv);
bool validateUser(const string &name, const string &password, const string &key, const string &iv);
#endif // LOGINANDJOIN_H

