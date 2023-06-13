#include <iostream>
#include <QApplication>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QWidget>
#include <QVBoxLayout>
#include <QMessageBox>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <QPixmap>

using namespace std;

class Character
{
private:
    string name;
    int strength;
    int speed;
    int agility;
    int experience;
    int level;
    string iconPath;

public:
    Character(string name = "", int strength = 0, int speed = 0, int agility = 0, int experience = 0, int level = 1)
        : name(name), strength(strength), speed(speed), agility(agility), experience(experience), level(level){}

    string getName() const { return name; }
    int getStrength() const { return strength; }
    int getSpeed() const { return speed; }
    int getAgility() const { return agility; }
    int getExperience() const { return experience; }
    int getLevel() const { return level; }

    void setName(const string& name) { this->name = name; }
    void setStrength(int strength) { this->strength = strength; }
    void setSpeed(int speed) { this->speed = speed; }
    void setAgility(int agility) { this->agility = agility; }
    void setExperience(int experience) { this->experience = experience; }
    void setLevel(int level) { this->level = level; }

    friend std::ostream& operator<<(std::ostream& os, const Character& character);
    friend std::istream& operator>>(std::istream& is, Character& character);

    void setIconPath(const std::string& path) { iconPath = path; }
    const std::string& getIconPath() const { return iconPath; }
};

std::ostream& operator<<(std::ostream& os, const Character& character)
{
    os << character.getName() << endl
       << character.getStrength() << endl
       << character.getSpeed() << endl
       << character.getAgility() << endl
       << character.getExperience() << endl
       << character.getLevel() << endl
       << character.getIconPath() << endl;
    return os;
}

std::istream& operator>>(std::istream& is, Character& character)
{
    std::string name;
    int strength, speed, agility, experience, level;
    std::string iconPath;

    std::getline(is, name);
    is >> std::ws >> strength >> std::ws >> speed >> std::ws >> agility >> std::ws >> experience >> std::ws >> level >> std::ws;
    std::getline(is, iconPath);

    character.setName(name);
    character.setStrength(strength);
    character.setSpeed(speed);
    character.setAgility(agility);
    character.setExperience(experience);
    character.setLevel(level);
    character.setIconPath(iconPath);

    return is;
}

void saveCharacter(const Character& character, const std::string& filename)
{
    std::ofstream file(filename);
    if (!file)
    {
        cerr << "Nie można otworzyć pliku do zapisu: " << filename << '\n';
        return;
    }

    file << character;
}

void loadCharacter(Character& character, const std::string& filename)
{
    std::ifstream file(filename);
    if (!file)
    {
        cerr << "Nie można otworzyć pliku do odczytu: " << filename << '\n';
        return;
    }

    file >> character;
}

void showCharacter(const Character* character)
{
    QWidget* characterWindow = new QWidget;
    characterWindow->setWindowTitle("Twoja postać");
    characterWindow->resize(350, 200);

    QGridLayout* layout = new QGridLayout(characterWindow);

    if (character->getName().empty())
    {
        QLabel* nameLabel = new QLabel("Nie stworzono jeszcze postaci");
        layout->addWidget(nameLabel, 0, 0, 1, 2);
    }
    else
    {
        QLabel* nameLabel = new QLabel(QString::fromStdString(character->getName()));
        QFont font = nameLabel->font();
        font.setBold(true);
        nameLabel->setFont(font);
        layout->addWidget(new QLabel("Nazwa postaci:"), 0, 0);
        layout->addWidget(nameLabel, 0, 1);

        layout->addWidget(new QLabel("Siła:"), 1, 0);
        layout->addWidget(new QLabel(QString::number(character->getStrength())), 1, 1);

        layout->addWidget(new QLabel("Szybkość:"), 2, 0);
        layout->addWidget(new QLabel(QString::number(character->getSpeed())), 2, 1);

        layout->addWidget(new QLabel("Zręczność:"), 3, 0);
        layout->addWidget(new QLabel(QString::number(character->getAgility())), 3, 1);

        layout->addWidget(new QLabel("Poziom:"), 4, 0);
        layout->addWidget(new QLabel(QString::number(character->getLevel())), 4, 1);

        layout->addWidget(new QLabel("Punkty doświadczenia:"), 5, 0);
        layout->addWidget(new QLabel(QString::number(character->getExperience())), 5, 1);

        if (!character->getIconPath().empty())
        {
            QPixmap pixmap(QString::fromStdString(character->getIconPath()));
            QLabel* iconLabel = new QLabel;
            iconLabel->setPixmap(pixmap.scaled(180, 180, Qt::KeepAspectRatio));
            layout->addWidget(iconLabel, 0, 2, 6, 1);
        }
    }

    characterWindow->setLayout(layout);
    characterWindow->show();
}

void createCharacter(Character* character)
{
    QWidget* characterWindow = new QWidget;
    characterWindow->setWindowTitle("Tworzenie postaci");
    characterWindow->resize(400, 220);

    QFormLayout* layout = new QFormLayout(characterWindow);

    QLineEdit* nameEdit = new QLineEdit;
    layout->addRow("Nazwa postaci:", nameEdit);

    QSpinBox* strengthEdit = new QSpinBox;
    strengthEdit->setRange(0, 10);
    layout->addRow("Siła:", strengthEdit);

    QSpinBox* speedEdit = new QSpinBox;
    speedEdit->setRange(0, 10);
    layout->addRow("Szybkość:", speedEdit);

    QSpinBox* agilityEdit = new QSpinBox;
    agilityEdit->setRange(0, 10);
    layout->addRow("Zręczność:", agilityEdit);

    QLabel* experienceLabel = new QLabel("0");
    layout->addRow("Doświadczenie:", experienceLabel);

    QLabel* levelLabel = new QLabel("1");
    layout->addRow("Poziom:", levelLabel);

    QPushButton* button = new QPushButton("Stwórz postać");
    layout->addRow(button);

    QObject::connect(button, &QPushButton::clicked, [characterWindow, character, nameEdit, strengthEdit, speedEdit, agilityEdit, experienceLabel, levelLabel]() {
        if (!character->getName().empty()) {
            QMessageBox msgBox;
            msgBox.setText("Postać jest już stworzona.");
            msgBox.setInformativeText("Czy chcesz utworzyć nową postać?");
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msgBox.setDefaultButton(QMessageBox::No);
            int ret = msgBox.exec();

            switch (ret) {
                case QMessageBox::Yes:
                    character->setName(nameEdit->text().toStdString());
                    character->setStrength(strengthEdit->value());
                    character->setSpeed(speedEdit->value());
                    character->setAgility(agilityEdit->value());
                    character->setExperience(0);
                    character->setLevel(1);
                    character->setIconPath("C:/Users/Piotrek/Documents/Studia/Semestr 6/POIO/Laboratoria/RPG/icons/character");
                    characterWindow->close();
                    showCharacter(character);
                    break;
                case QMessageBox::No:
                    characterWindow->close();
                    break;
            }
        }
        else {
            character->setName(nameEdit->text().toStdString());
            character->setStrength(strengthEdit->value());
            character->setSpeed(speedEdit->value());
            character->setAgility(agilityEdit->value());
            character->setExperience(0);
            character->setLevel(1);
            character->setIconPath("C:/Users/Piotrek/Documents/Studia/Semestr 6/POIO/Laboratoria/RPG/icons/character");
            characterWindow->close();
            showCharacter(character);
        }

        delete characterWindow;
    });

    characterWindow->show();
}

void generateEnemy(Character* enemy, int characterLevel)
{
    srand(time(0));
    QString enemy_names[5] = {"Ork", "Łotr", "Smok", "Szkieletor", "Zjawa"};
    enemy->setName(enemy_names[rand() % 5].toStdString());
    enemy->setStrength((rand() % 10 + 1) * characterLevel);
    enemy->setSpeed((rand() % 10 + 1) * characterLevel);
    enemy->setAgility((rand() % 10 + 1) * characterLevel);

    enemy->setLevel(characterLevel);
    enemy->setIconPath("C:/Users/Piotrek/Documents/Studia/Semestr 6/POIO/Laboratoria/RPG/icons/" + enemy->getName());
}

void showEnemy(const Character* enemy)
{
    QWidget* enemyWindow = new QWidget;
    enemyWindow->setWindowTitle("Twój przeciwnik");
    enemyWindow->resize(350, 200);

    QGridLayout* layout = new QGridLayout(enemyWindow);

    if (enemy->getName().empty())
    {
        QLabel* nameLabel = new QLabel("Nie wygenerowano jeszcze przeciwnika");
        layout->addWidget(nameLabel, 0, 0, 1, 2);
    }
    else
    {
        QLabel* nameLabel = new QLabel(QString::fromStdString(enemy->getName()));
        QFont font = nameLabel->font();
        font.setBold(true);
        nameLabel->setFont(font);
        layout->addWidget(new QLabel("Nazwa postaci:"), 0, 0);
        layout->addWidget(nameLabel, 0, 1);

        layout->addWidget(new QLabel("Siła:"), 1, 0);
        layout->addWidget(new QLabel(QString::number(enemy->getStrength())), 1, 1);

        layout->addWidget(new QLabel("Szybkość:"), 2, 0);
        layout->addWidget(new QLabel(QString::number(enemy->getSpeed())), 2, 1);

        layout->addWidget(new QLabel("Zręczność:"), 3, 0);
        layout->addWidget(new QLabel(QString::number(enemy->getAgility())), 3, 1);

        layout->addWidget(new QLabel("Poziom:"), 4, 0);
        layout->addWidget(new QLabel(QString::number(enemy->getLevel())), 4, 1);

        if (!enemy->getIconPath().empty())
        {
            QPixmap pixmap(QString::fromStdString(enemy->getIconPath()));
            QLabel* iconLabel = new QLabel;
            iconLabel->setPixmap(pixmap.scaled(180, 180, Qt::KeepAspectRatio));
            layout->addWidget(iconLabel, 0, 2, 5, 1);
        }
    }

    enemyWindow->setLayout(layout);
    enemyWindow->show();
}


void fight(Character* player, const Character* enemy)
{

    QWidget* fightWindow = new QWidget;
    fightWindow->setWindowTitle("Walka");
    fightWindow->resize(500, 300);

    QHBoxLayout* mainLayout = new QHBoxLayout(fightWindow);

    QFont font;
    font.setBold(true);

    QVBoxLayout* playerLayout = new QVBoxLayout;
    QLabel* playerNameLabel = new QLabel(QString::fromStdString(player->getName()));
    playerNameLabel->setFont(font);
    QLabel* playerLabel = new QLabel("Siła: " + QString::number(player->getStrength()) + "\nSzybkość: " + QString::number(player->getSpeed()) + "\nZręczność: " + QString::number(player->getAgility()) + "\nPoziom: " + QString::number(player->getLevel()));
    QLabel* playerIconLabel = new QLabel;
    QPixmap playerIconPixmap(QString::fromStdString(player->getIconPath()));
    playerIconLabel->setPixmap(playerIconPixmap.scaled(180, 180, Qt::KeepAspectRatio));

    playerLayout->addWidget(playerNameLabel);
    playerLayout->addWidget(playerLabel);
    playerLayout->addWidget(playerIconLabel);

    QVBoxLayout* enemyLayout = new QVBoxLayout;
    QLabel* enemyNameLabel = new QLabel(QString::fromStdString(enemy->getName()));
    enemyNameLabel->setFont(font);
    QLabel* enemyLabel = new QLabel("Siła: " + QString::number(enemy->getStrength()) + "\nSzybkość: " + QString::number(enemy->getSpeed()) + "\nZręczność: " + QString::number(enemy->getAgility()) + "\nPoziom: " + QString::number(enemy->getLevel()));
    QLabel* enemyIconLabel = new QLabel;
    QPixmap enemyIconPixmap(QString::fromStdString(enemy->getIconPath()));
    enemyIconLabel->setPixmap(enemyIconPixmap.scaled(180, 180, Qt::KeepAspectRatio));

    enemyLayout->addWidget(enemyNameLabel);
    enemyLayout->addWidget(enemyLabel);
    enemyLayout->addWidget(enemyIconLabel);

    QVBoxLayout* vsLayout = new QVBoxLayout;
    QLabel* vsLabel = new QLabel;
    QPixmap vsPixmap("C:/Users/Piotrek/Documents/Studia/Semestr 6/POIO/Laboratoria/RPG/icons/vs");
    vsLabel->setPixmap(vsPixmap.scaled(150, 150, Qt::KeepAspectRatio));
    QPushButton* button = new QPushButton("Walcz!");

    vsLayout->addWidget(vsLabel);
    vsLayout->addWidget(button);

    QObject::connect(button, &QPushButton::clicked, [player, enemy, fightWindow]() {
        int playerTotal = player->getStrength() + player->getSpeed() + player->getAgility();
        int enemyTotal = enemy->getStrength() + enemy->getSpeed() + enemy->getAgility();

        QString winner;
        if (playerTotal > enemyTotal) {
            winner = "Wygrałeś! Zdobyłeś 50 punktów doświadczenia.";
            player->setExperience(player->getExperience() + 50);
        } else if (enemyTotal > playerTotal) {
            winner = "Przegrałeś!";
        } else {
            winner = "Remis!";
        }

        QMessageBox::information(fightWindow, "Wynik walki", winner);

        fightWindow->close();

        if (player->getExperience() >= 100)
        {
            player->setLevel(player->getLevel() + 1);

            QWidget* characterWindow = new QWidget;
            characterWindow->setWindowTitle("Upgrade postaci");
            characterWindow->resize(350, 200);

            QFormLayout* layout = new QFormLayout(characterWindow);

            QLabel* nameLabel = new QLabel(QString::fromStdString(player->getName()));
            QFont font = nameLabel->font();
            font.setBold(true);
            nameLabel->setFont(font);

            QSpinBox* strengthEdit = new QSpinBox;
            strengthEdit->setRange(player->getStrength(), 20);
            layout->addRow("Siła:", strengthEdit);

            QSpinBox* speedEdit = new QSpinBox;
            speedEdit->setRange(player->getSpeed(), 20);
            layout->addRow("Szybkość:", speedEdit);

            QSpinBox* agilityEdit = new QSpinBox;
            agilityEdit->setRange(player->getAgility(), 20);
            layout->addRow("Zręczność:", agilityEdit);

            QLabel* experienceLabel = new QLabel(QString::number(player->getExperience()));
            layout->addRow("Doświadczenie:", experienceLabel);

            QLabel* levelLabel = new QLabel(QString::number(player->getLevel()));
            layout->addRow("Poziom:", levelLabel);

            QPushButton* button = new QPushButton("Ulepsz postać");
                QObject::connect(button, &QPushButton::clicked, [player, strengthEdit, speedEdit, agilityEdit, characterWindow]() {
                    player->setStrength(strengthEdit->value());
                    player->setSpeed(speedEdit->value());
                    player->setAgility(agilityEdit->value());
                    player->setExperience(player->getExperience() - 100);
                    characterWindow->close();
                });
            layout->addRow(button);

            characterWindow->setLayout(layout);
            characterWindow->show();
        }

    });

    mainLayout->addLayout(playerLayout);
    mainLayout->addLayout(vsLayout);
    mainLayout->addLayout(enemyLayout);

    fightWindow->show();
}





int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Character character;
    Character enemy;

    QWidget menu;
    menu.setWindowTitle("Menu gry");
    menu.resize(400, 300);
    QLabel *game_hello = new QLabel("Witaj w grze RPG", &menu);
    QPushButton *button1 = new QPushButton("Stwórz nową postać", &menu);
    QPushButton *button2 = new QPushButton("Wyświetl postać", &menu);
    QPushButton *button3 = new QPushButton("Wygeneruj przeciwnika", &menu);
    QPushButton *button5 = new QPushButton("Walcz", &menu);
    QPushButton *button6 = new QPushButton("Zapisz postać", &menu);
    QPushButton *button7 = new QPushButton("Wczytaj postać", &menu);
    QVBoxLayout *menu_layout = new QVBoxLayout(&menu);
    menu_layout->addWidget(game_hello);
    menu_layout->addWidget(button1);
    menu_layout->addWidget(button2);
    menu_layout->addWidget(button3);
    menu_layout->addWidget(button5);
    menu_layout->addWidget(button6);
    menu_layout->addWidget(button7);

    QObject::connect(button1, &QPushButton::clicked, [&]() {
        createCharacter(&character);
    });

    QObject::connect(button2, &QPushButton::clicked, [&]() {
        showCharacter(&character);
    });

    QObject::connect(button3, &QPushButton::clicked, [&]() {
        generateEnemy(&enemy, character.getLevel());
        showEnemy(&enemy);
    });

    QObject::connect(button5, &QPushButton::clicked, [&]() {
        generateEnemy(&enemy, character.getLevel());
        fight(&character, &enemy);
    });

    QObject::connect(button6, &QPushButton::clicked, [&]() {
        saveCharacter(character, "savedCharacter.txt");
    });

    QObject::connect(button7, &QPushButton::clicked, [&]() {
        loadCharacter(character, "savedCharacter.txt");
        showCharacter(&character);
    });

    menu.show();

    return app.exec();
}
