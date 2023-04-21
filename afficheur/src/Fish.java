

public class Fish {

    String name;
    Rectangle virtualRec;
    boolean isStarted;
    Move move;
    int x;
    int y;
    

    public Fish(String name, Rectangle vR, int x, int y) {
        this.name = name;
        this.virtualRec = vR;
        this.x = x;
        this.y = y;
    }

    public String getName() {
        return this.name;
    }

    public Rectangle getVirtualRec() {
        return this.virtualRec;
    }

    public showFish() {
        String status_1 = "Fish " + this.name + "at" + this.x + "x" + this.y;
        String status_2 = this.virtualRec.show();
        String status_3 = this.move.name;
        String status_4 = isStarted ? "Started" : "not Started";
        System.out.println(status_1 + ", " + status_2 + ", " + status_3 + ", " + status_4);
    }


}
