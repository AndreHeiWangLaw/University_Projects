// Andre Hei Wang Law
// 4017 5600

package ca.concordia.coen346.fridgethreads;

public class Fridge {

    private boolean milk;

    public Fridge(){
        this.milk = false;
    }

    public boolean gotMilk(){
        return milk;
    }

    public void drinkMilk(){
        if(this.milk)
            this.milk = false;
    }

    public void putMilk() throws Exception{
        if(gotMilk()){
            throw new Exception("Too much milk!");
        }
        this.milk = true;
    }


}
