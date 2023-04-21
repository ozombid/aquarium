

public class Aquarium {

    String name;
    Fish[] fishes;
    int numFishes;

    public Aquarium(String name, Fish[] fishes, int numFishes) {
        this.name = name;
        this.fishes = fishes;
        this.numFishes = numFishes;
    }

    public void addFish(Fish fish) {
        this.fishes[this.numFishes] = fish;
        this.numFishes++;
    }

    public int findFish(Fish fish) {
        for (int i=0; i<numFishes; i++) {
            if (this.fishes[i].getName() == fish.getName())
                return i;
        }
        return -1;
    }

    private sh

    public String delFish(Fish fish) {
        int idx = this.findFish(fish);
        if (idx == -1)
            return "no fish found";
        else 
            

        this.fishes[this.numFishes] = fish;
        this.numFishes++;
    }

} 