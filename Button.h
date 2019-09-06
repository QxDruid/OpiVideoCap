class Button
{
public:
    Button(int Pin_);
    ~Button(){}

    bool isPressed(void);
private:
    int Pin;
};
