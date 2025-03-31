#include "./../include/headers.hpp"
using namespace std;
using json = nlohmann::json;
using namespace CryptoPP;

void loadEnvFile(const string &filename);
string getEnvVar(const string &key);
string encryptAES(const string &plainText, const string &key, const string &iv);
string decryptAES(const string &cipherText, const string &key, const string &iv);
unsigned long generateUserId(const string &name);
void saveEncryptedJson(const json &data, const string &key, const string &iv);
json loadEncryptedJson(const string &key, const string &iv);
void createUser(const string &name, const string &lastName, const string &password, const string &key, const string &iv);
bool validateUser(const string &username, const string &password, const string &key, const string &iv);


// Función para cargar el archivo .env
void loadEnvFile(const string &filename)
{
    ifstream file(filename);
    if (!file)
    {
        cerr << "Error: No se pudo abrir el archivo " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line))
    {
        if (line.empty() || line[0] == '#') // Ignorar comentarios
            continue;

        istringstream is_line(line);
        string key, value;
        if (getline(is_line, key, '=') && getline(is_line, value))
        {
            setenv(key.c_str(), value.c_str(), 1); // Cargar en variables de entorno
        }
    }
}

// Obtener claves desde variables de entorno
string getEnvVar(const string &key)
{
    const char *val = getenv(key.c_str());
    return val == nullptr ? "" : string(val);
}

// Función para cifrar un texto con AES
string encryptAES(const string &plainText, const string &key, const string &iv)
{
    string cipherText;
    try
    {
        CBC_Mode<AES>::Encryption encryption;
        encryption.SetKeyWithIV((CryptoPP::byte *)key.c_str(), AES::DEFAULT_KEYLENGTH, (CryptoPP::byte *)iv.c_str());

        StringSource(plainText, true,
                     new StreamTransformationFilter(encryption,
                                                    new StringSink(cipherText),
                                                    BlockPaddingSchemeDef::PKCS_PADDING));
    }
    catch (const Exception &e)
    {
        cerr << "Error en cifrado: " << e.what() << endl;
    }
    return cipherText;
}

// Función para descifrar un texto con AES
string decryptAES(const string &cipherText, const string &key, const string &iv)
{
    string plainText;
    try
    {
        CBC_Mode<AES>::Decryption decryption;
        decryption.SetKeyWithIV((CryptoPP::byte *)key.c_str(), AES::DEFAULT_KEYLENGTH, (CryptoPP::byte *)iv.c_str());

        StringSource(cipherText, true,
                     new StreamTransformationFilter(decryption,
                                                    new StringSink(plainText),
                                                    BlockPaddingSchemeDef::PKCS_PADDING));
    }
    catch (const Exception &e)
    {
        cerr << "Error en descifrado: " << e.what() << endl;
    }
    return plainText;
}

// Función para generar un ID a partir del nombre
unsigned long generateUserId(const string &name)
{
    unsigned long user_id = 0;
    for (size_t i = 0; i < name.size(); ++i)
    {
        user_id = user_id * 31 + name[i];
    }
    return user_id;
}

// Función para guardar los datos en un archivo JSON cifrado
void saveEncryptedJson(const json &data, const string &key, const string &iv)
{
    ofstream file("./json/users.json", ios::binary);
    if (!file)
    {
        cerr << "Error al abrir el archivo" << endl;
        return;
    }
    string encryptedData = encryptAES(data.dump(), key, iv);
    file << encryptedData;
    file.close();
}

// Función para leer y descifrar el JSON
json loadEncryptedJson(const string &key, const string &iv)
{
    ifstream file("./json/users.json", ios::binary);
    if (!file)
    {
        return json::array(); // Retorna un JSON vacío si no existe
    }

    string encryptedData((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    string decryptedData = decryptAES(encryptedData, key, iv);
    return json::parse(decryptedData);
}

// Función para registrar un usuario
void createUser(const string &name, const string &lastName, const string &password, const string &key, const string &iv)
{
    json users = loadEncryptedJson(key, iv);

    unsigned long user_id = generateUserId(name);

    // Crear el nuevo usuario
    json newUser;
    newUser["user_id"] = user_id;
    newUser["name"] = name;
    newUser["last_name"] = lastName;
    newUser["password"] = password;
    newUser["user_auth"] = "user";

    // Agregar el usuario a la lista y guardar el archivo cifrado
    users.push_back(newUser);
    saveEncryptedJson(users, key, iv);

    cout << "Usuario creado exitosamente!\n"
         << endl;
}

// Función para validar usuario y contraseña
bool validateUser(const string &username, const string &password, const string &key, const string &iv)
{
    json users = loadEncryptedJson(key, iv);

    unsigned long user_id = generateUserId(username);

    for (const auto &user : users)
    {
        if (user["user_id"] == user_id && user["password"] == password)
        {
            cout << "Inicio de sesión exitoso\n"
                 << endl;
            return true;
        }
    }
    mvprintw(9, 10, "Error: Usuario o contraseña invalida, presione ENTER.");
    return false;
}
