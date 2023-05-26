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

using namespace std;

class Character
{
private:
    string name;
    int strength;
    int speed;
    int agility;

public:
    Character(string name = "", int strength = 0, int speed = 0, int agility = 0)
        : name(name), strength(strength), speed(speed), agility(agility) {}

    void setName(const string& name) { this->name = name; }
    void setStrength(int strength) { this->strength = strength; }
    void setSpeed(int speed) { this->speed = speed; }
    void setAgility(int agility) { this->agility = agility; }

    string getName() const { return name; }
    int getStrength() const { return strength; }
    int getSpeed() const { return speed; }
    int getAgility() const { return agility; }
};


void createCharacter(Character* character) {
    QWidget* characterWindow = new QWidget;
    characterWindow->setWindowTitle("Tworzenie postaci");
    characterWindow->resize(400,170);

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

    QPushButton* button = new QPushButton("Stwórz postać");
    layout->addRow(button);

    QObject::connect(button, &QPushButton::clicked, [characterWindow, character, nameEdit, strengthEdit, speedEdit, agilityEdit]() {
        if (!character->getName().empty()) {
            QMessageBox msgBox;
            msgBox.setText("Postać jest już stworzona.");
            msgBox.setInformativeText("Czy chcesz utworzyć nową postać?");
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msgBox.setDefaultButton(QMessageBox::No);
            int ret = msgBox.exec();

            switch (ret) {
                case QMessageBox::Yes:
                    // Utwórz nową postać
                    character->setName(nameEdit->text().toStdString());
                    character->setStrength(strengthEdit->value());
                    character->setSpeed(speedEdit->value());
                    character->setAgility(agilityEdit->value());
                    characterWindow->close();
                    break;
                case QMessageBox::No:
                    // Zamknij okno tworzenia postaci
                    characterWindow->close();
                    break;
            }
        }
        else {
            // Utwórz postać jeśli nie istnieje
            character->setName(nameEdit->text().toStdString());
            character->setStrength(strengthEdit->value());
            character->setSpeed(speedEdit->value());
            character->setAgility(agilityEdit->value());
            characterWindow->close();
        }

        delete characterWindow; // Zwolnienie pamięci
    });


    characterWindow->show();
}



void generateEnemy(Character *enemy)
{
    srand(time(0));
    QString enemy_names[5] = {"Ork", "Łotr", "Smok", "Szkieletor", "Zjawa"};
    enemy->setName(enemy_names[rand() % 5].toStdString());
    enemy->setStrength(rand() % 10 + 1);
    enemy->setSpeed(rand() % 10 + 1);
    enemy->setAgility(rand() % 10 + 1);
}


void showCharacter(const Character* character) {
    QWidget* characterWindow = new QWidget;
    characterWindow->setWindowTitle("Twoja postać");
    characterWindow->resize(250, 100);

    QFormLayout* layout = new QFormLayout(characterWindow);

    if(character->getName().empty()) {
        QLabel* nameLabel = new QLabel("Nie stworzono jeszcze postaci");
        layout->addRow(nameLabel);
    }
    else {
        QLabel* nameLabel = new QLabel(QString::fromStdString(character->getName()));
        layout->addRow("Nazwa postaci:", nameLabel);

        QLabel* strengthLabel = new QLabel(QString::number(character->getStrength()));
        layout->addRow("Siła:", strengthLabel);

        QLabel* speedLabel = new QLabel(QString::number(character->getSpeed()));
        layout->addRow("Szybkość:", speedLabel);

        QLabel* agilityLabel = new QLabel(QString::number(character->getAgility()));
        layout->addRow("Zręczność:", agilityLabel);
    }

    characterWindow->show();
}

void showEnemy(const Character* enemy) {
    QWidget* enemyWindow = new QWidget;
    enemyWindow->setWindowTitle("Twój przeciwnik");
    enemyWindow->resize(250, 100);

    QFormLayout* layout = new QFormLayout(enemyWindow);

    if(enemy->getName().empty()) {
        QLabel* nameLabel = new QLabel("Nie wygenerowano jeszcze przeciwnika");
        layout->addRow(nameLabel);
    }
    else {
        QLabel* nameLabel = new QLabel(QString::fromStdString(enemy->getName()));
        layout->addRow("Nazwa postaci:", nameLabel);

        QLabel* strengthLabel = new QLabel(QString::number(enemy->getStrength()));
        layout->addRow("Siła:", strengthLabel);

        QLabel* speedLabel = new QLabel(QString::number(enemy->getSpeed()));
        layout->addRow("Szybkość:", speedLabel);

        QLabel* agilityLabel = new QLabel(QString::number(enemy->getAgility()));
        layout->addRow("Zręczność:", agilityLabel);
    }

    enemyWindow->show();
}


void fight(const Character *player, const Character *enemy) {
    QWidget* fightWindow = new QWidget;
    fightWindow->setWindowTitle("Walka");
    fightWindow->resize(500, 200);

    QFormLayout* layout = new QFormLayout(fightWindow);

    QLabel* playerLabel = new QLabel("Twoja postać:\n" + QString::fromStdString(player->getName()) + "\nSiła: " + QString::number(player->getStrength()) + "\nSzybkość: " + QString::number(player->getSpeed()) + "\nZręczność: " + QString::number(player->getAgility()));
    QLabel* vsLabel = new QLabel("VS");
    QLabel* enemyLabel = new QLabel("Przeciwnik:\n" + QString::fromStdString(enemy->getName()) + "\nSiła: " + QString::number(enemy->getStrength()) + "\nSzybkość: " + QString::number(enemy->getSpeed()) + "\nZręczność: " + QString::number(enemy->getAgility()));

    QHBoxLayout* fightLayout = new QHBoxLayout;
    fightLayout->addWidget(playerLabel);
    fightLayout->addWidget(vsLabel);
    fightLayout->addWidget(enemyLabel);

    layout->addRow(fightLayout);

    QPushButton* button = new QPushButton("Walcz!");
    layout->addRow(button);

    QObject::connect(button, &QPushButton::clicked, [player, enemy, fightWindow]() {
        int playerTotal = player->getStrength() + player->getSpeed() + player->getAgility();
        int enemyTotal = enemy->getStrength() + enemy->getSpeed() + enemy->getAgility();

        QString winner;
        if (playerTotal > enemyTotal) {
            winner = "Wygrałeś!";
        } else if (enemyTotal > playerTotal) {
            winner = "Przegrałeś!";
        } else {
            winner = "Remis!";
        }

        QMessageBox::information(fightWindow, "Wynik walki", winner);

        fightWindow->close();
    });

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
    QPushButton *button4 = new QPushButton("Wyświetl przeciwnika", &menu);
    QPushButton *button5 = new QPushButton("Walcz", &menu);
    QVBoxLayout *menu_layout = new QVBoxLayout(&menu);
    menu_layout->addWidget(game_hello);
    menu_layout->addWidget(button1);
    menu_layout->addWidget(button2);
    menu_layout->addWidget(button3);
    menu_layout->addWidget(button4);
    menu_layout->addWidget(button5);

    QObject::connect(button1, QPushButton::clicked, [&]() {
    createCharacter(&character);
    });

    QObject::connect(button2, QPushButton::clicked, [&]()
    {
    showCharacter(&character);
    });

    QObject::connect(button3, QPushButton::clicked, [&]()
    {
    generateEnemy(&enemy);
    });

    QObject::connect(button4, QPushButton::clicked, [&]()
    {
    showEnemy(&enemy);
    });

    QObject::connect(button5, QPushButton::clicked, [&]()
    {
    fight(&character, &enemy);
    });

    menu.show();

    return app.exec();
}
