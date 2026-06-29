

class PowerModule {
public:
    static PowerModule& getInstance() {
        static PowerModule instance;
        return instance;
    }
    
private:
    PowerModule(){}
};