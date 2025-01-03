
#include "pch.h"
#include "AccountManager.h"

AccountManager::AccountManager()
    : username(""), password(""), health(3), fireRate(2000),
    points(0), score(0), isFireRateUpgrade(false), isSpeedUpgrade(false), bulletSpeed(0.25) {}

AccountManager::AccountManager(const std::string& user, const std::string& pass, size_t heal, 
    uint32_t fRate, uint16_t pts, uint16_t scr, bool iFRU, bool iSU, double bSpeed)
    : username(user), password(pass), health(heal), fireRate(fRate),
    points(pts), score(scr), isFireRateUpgrade(iFRU), isSpeedUpgrade(iSU), bulletSpeed(bSpeed) {}

void AccountManager::SetUsername(const std::string& user) { username = user; }
void AccountManager::SetPassword(const std::string& pass) { password = pass; }
void AccountManager::SetHealth(size_t& heal) { health =heal; }
void AccountManager::SetFireRate(uint32_t& fRate) { fireRate = fRate; }
void AccountManager::SetPoints(uint16_t& pts) { points = pts; }
void AccountManager::SetBulletSpeed(double& bSpeed) { bulletSpeed = bSpeed; }

std::string AccountManager::GetUsername() const { return username; }
std::string AccountManager::GetPassword() const { return password; }
uint16_t AccountManager::GetPoints() const { return points; }
uint16_t AccountManager::GetScore() const { return score; }

std::chrono::milliseconds AccountManager::GetFireRate() const { return std::chrono::milliseconds(fireRate); }

bool AccountManager::GetSpeedBoost() const { return isFireRateUpgrade; }
bool AccountManager::GetSpeedUpgrade() const { return isSpeedUpgrade; }

bool AccountManager::Authenticate(const std::string& user, const std::string& pass) const {
    return username == user && password == HashPassword(pass);
}

inline auto AccountManager::InItStorage(const std::string& dbFile) const
{
    using namespace sqlite_orm;
    return make_storage(
        dbFile,
        make_table("Account",
            make_column("Username", &AccountManager::username, primary_key()),
            make_column("Password", &AccountManager::password),
            make_column("Health", &AccountManager::health),
            make_column("FireRate", &AccountManager::fireRate),
            make_column("Points", &AccountManager::points),
            make_column("Score", &AccountManager::score),
            make_column("isFireRateUpgrade", &AccountManager::isFireRateUpgrade),
            make_column("isSpeedUpgrade", &AccountManager::isSpeedUpgrade),
            make_column("BulletSpeed", &AccountManager::bulletSpeed))
    );
}

void AccountManager::SaveDataToDatabase(const std::string& dbFile) const
{
    auto storage = InItStorage(dbFile);
    storage.sync_schema();
    storage.replace(*this);
}


void AccountManager::LoadDataFromDatabase(const std::string& dbFile, const std::string& user) {
    auto storage = InItStorage(dbFile);
    storage.sync_schema();

    auto account = storage.get_pointer<AccountManager>(user);
    if (account) {
        *this = *account;
    }
    else {
        std::cerr << "No data found for user: " << user << std::endl;
    }
}

void AccountManager::SignUp(const std::string& dbFile, const std::string& user, const std::string& pass)
{
    if (!IsValidPassword(pass)) {
        std::cerr << "Password must contain at least one uppercase letter, one lowercase letter, one digit, one special character, and be at least 8 characters long.\n";
        return;
    }

    auto storage = InItStorage(dbFile);
    storage.sync_schema();

    auto existingAccount = storage.get_pointer<AccountManager>(user);

    if(existingAccount){
        std::cerr << "Account already exists.Please chose another one. "<< std::endl;
        return;
    }
    AccountManager newAccount(user, HashPassword(pass), 3, 2000, 0, 0, false, false, 0.25);
    storage.replace(newAccount);
    std::cout << " Account created succesfully!" << std::endl;
}

void AccountManager::LoginForm(const std::string& dbFile)
{
    AccountManager account;
    while (true) {
        std::cout << "\n....LoginForm....\n";
        std::cout << "1.Login\n";
        std::cout << "2.Sign up\n";
        std::cout << "3.Exit\n";
        std::cout << "\nChose your option : \n";

        int choice;
        std::cin >> choice;

        switch (choice) {
        case 1:
            std::cout << "Enter Username: ";
            std::cin >> username;
            std::cout << "Enter Password: ";
            std::cin >> password;

            account.LoadDataFromDatabase(dbFile, username);
            if (account.Authenticate(username, password)) {
                std::cout << "Login successful! Welcome, " << username << "!\n";
            }
            else {
                std::cout << "Invalid credentials. Please try again.\n";
            }
            break;

        case 2:

            std::cout << "Choose a Username: ";
            std::cin >> username;
            std::cout << "Password must contain minim 8 caracters one uppercase one lowercase one special caracter (@, $, !, %, *, ?, &, #)\n" << "Choose a Password: ";
            std::cin >> password;

            account.SignUp(dbFile, username, password);
            break;

        case 3:
            std::cout << "Exiting...\n";
            return;

        default:
            std::cout << "Invalid option. Please try again.\n";

        }

    }
}



bool AccountManager::IsValidPassword(const std::string& pass) const
{
    //Parola trb sa contina minim 8 caractere: min 1 Uppercase,Lowercase,si caracterele : @ $ ! % * ? & #
    std::regex passRegex("^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d)(?=.*[@$!%*?&#])[A-Za-z\\d@$!%*?&#]{8,}$");

    return std::regex_match(pass, passRegex);
}

std::string AccountManager::HashPassword(const std::string& pass) const
{
    std::hash<std::string> hasher;
    size_t hashed = hasher(pass);
    std::stringstream ss;
    ss << std::hex << hashed;
    return ss.str();
}

AccountManager::~AccountManager()
{
}
