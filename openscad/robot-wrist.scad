include <gears.scad>

gear2_bolt_hex_d=12.8;
gear2_bolt_sink=4;



h=5;

modul=1.5;
top_tooth_number=53;
rt_tooth_number=top_tooth_number;
small_tooth_number=11;
a=modul*(2+top_tooth_number);

b=modul*(2+rt_tooth_number);
aa=modul*(2+small_tooth_number);

db=-aa/2+2*(modul+modul/6);

c=a;
rtube=2.54*10;
echo(c);
tol=.4;

smallgearoffset=.6;


////////////////////////////////////////////////////////////
nema=0;
module big_gear(modul=1.5){
	bevel_herringbone_gear(
	    modul=modul,
	    tooth_number=top_tooth_number,
	    partial_cone_angle=45,
	    tooth_width=10,
	    bore=4,
	    pressure_angle=20,
	    helix_angle=-20);
    }


module top_gear(modul=1.5,m=11){
    difference(){
	union(){
	    bevel_herringbone_gear(
		modul=modul,
		tooth_number=top_tooth_number,
		partial_cone_angle=45,
		tooth_width=10,
		bore=4,
		pressure_angle=20,
		helix_angle=20);

	    translate([0,0,-7]){
	    cylinder(h=(10+2*modul)*sin(45)+7,r=16,center=false);
	    cylinder(h=7,r1=a/2-2*modul-7,r2=a/2-sqrt(2)*modul,center=false);
	}
	    
    }
    
    translate([0,0,(10+2*modul)*sin(45)-7])
    cylinder(h=7+.5,r=11);
    
    translate([0,0,-3.5])	
    cylinder(h=7+.5,r=11,center=true);
    
    cylinder(h=100,r=11-.32,center=true);


    //holes
    for(i=[0:m-1]){
	rotate([0,0,i*360/m])
	translate([0,a/3.5,0])
	cylinder(h=20,r=4,center=true);}
}
}

module bottom_gear(modul=1.5,m=11){
    difference(){
	union(){
	    bevel_herringbone_gear(
		modul=modul,
		tooth_number=top_tooth_number,
		partial_cone_angle=45,
		tooth_width=10,
		bore=4,
		pressure_angle=20,
		helix_angle=20);
	    
	    translate([0,0,-7]){
		cylinder(h=(10+2*modul)*sin(45)+7,r=16,center=false);
	    }

	    translate([0,0,-7]){
	    cylinder(h=(10+2*modul)*sin(45)+7,r=16,center=false);
	    cylinder(h=7,r1=a/2-2*modul-7,r2=a/2-sqrt(2)*modul,center=false);
	}
    }
    
    translate([0,0,(10+2*modul)*sin(45)-7])
    cylinder(h=7+.5,r=11);
    
    translate([0,0,-3.5])	
    cylinder(h=7+.5,r=11,center=true);
    
    cylinder(h=100,r=11-.32,center=true);
    
    //holes
    for(i=[0:m-1]){
	rotate([0,0,i*360/m])
	translate([0,a/3.5,0])
	cylinder(h=20,r=4,center=true);}
}

height=50;
translate([0,0,-height-.01])
base(height=height,screws=true);
}

module lr_big_gear(modul=1.5,m=11){
    difference(){
	union(){
	    difference(){
		big_gear();
		translate([0,0,3.5+.4])
		cylinder(h=7,r=11,center=true);
		
		cylinder(h=100,r=11-.32,center=true);}
	    
	    translate([0,0,-10])
	    difference(){
		herringbone_gear(
		    modul=modul,
		    tooth_number=rt_tooth_number,
		    width=10,
		    bore=2*(11-.32),
		    pressure_angle=20,
		    helix_angle=30,
		    optimized=false);
		
		translate([0,0,3.5])
		cylinder(h=7,r=11,center=true);}}

        //holes
    for(i=[0:m-1]){
	rotate([0,0,i*360/m])
	translate([0,a/3.5,0])
	cylinder(h=20,r=4,center=true);}
}
}

module small_gear(modul=1.5){
    translate([0,a/2+smallgearoffset,-7]){
	rotate([0,0,-9])
	union(){
	    herringbone_gear(
		modul=modul,
		tooth_number=small_tooth_number,
		width=10,
		bore=5,
		pressure_angle=20,
		helix_angle=-30,
		optimized=false);
	}
	
	translate([0,0,-10])
	difference(){
	    cylinder(h=10,r=aa/2);
	    cylinder(h=20,r=2.5,center=true);
	    
	    translate([0,0,5])
	    rotate([0,90,0])
	    cylinder(h=20,r=1.5,center=true);
	    
	    translate([-5,0,5])
	    cube([2.8,5.6,10+.1],center=true);
	}
    }
	
}



module show(){
    translate([0,0,a/2])
    rotate([0,180,0])
    top_gear(modul=modul);
    
    translate([-a/2,0,0])
    rotate([0,90,0])
    lr_big_gear();
    
    union(){
	translate([a/2,0,0])
	rotate([0,-90,])
	lr_big_gear();
	
	translate([a/2+3,0,db])
	rotate([-90+0,0,0])
	rotate([0,-90,0])
	small_gear();}
    
    middleU();
    
    translate([-a/2-10*cos(45)-10-1.6,0,0]) lateral();
    
    translate([-a/2-3,0,db])
    rotate([-90,0,180])
    rotate([0,-90,0])
    small_gear(modul);
    
    
    translate([a/2+10*cos(45)+10+1.6,0,0])    rotate([0,0,180])    lateral();

    if(nema==1){
	// NEMA
	translate([-a/2-20-7,0,-a/2+db])
	rotate([0,90,0])
	nema17();
	
	// NEMA
	rotate([0,0,180])
	translate([-a/2-20-7,0,-a/2+db])
	rotate([0,90,0])
	nema17();}
	
	translate([0,0,a/2+25])    arm1(height=50-15);

    translate([0,0,-a/2])
    bottom_gear(modul=modul);

//    translate([0,0,a/2+200])
//    nema17();
}

show();
    


module middleU(c=a-10/sin(45)-2*1.6){
    rsphere=.65*c;
    lcube=c-4*7-1;
    difference(){
	intersection(){
	    cube(c*[1,1,1],center=true);
	    sphere(r=rsphere);}
	
	cube([lcube,100,lcube],center=true);
	
	cylinder(h=100,r=11-.32,center=true);
	rotate([0,90,0])
	cylinder(h=100,r=11-.32,center=true);

	// bearing hole
	for(i=[0:3]){
	    rotate([0,360*i/4,0]){
		    translate([0,0,c/2-3]) 
		    cylinder(h=8,r=11,center=true);
		    translate([0,0,c/2-2*7+2])
		    cylinder(h=8,r=11,center=true);}}}
    
    intersection(){
	sphere(r=rsphere);
	union(){
	    translate([lcube/2,0,lcube/2])
	    difference(){
		rotate([0,90,90])
		cylinder(h=c,r=7/2,center=true);
		translate([-7/2,0,-7/2])
		rotate([0,90,90])
		cylinder(h=c+1,r=7/2,center=true);}
	    
	    translate([-lcube/2,0,lcube/2])
	    difference(){
		rotate([0,90,90])
		cylinder(h=c,r=7/2,center=true);
		translate([7/2,0,-7/2])
		rotate([0,90,90])
		cylinder(h=c+1,r=7/2,center=true);}

	    ////////////////////////////////////////////////////////////
	    
	    translate([-lcube/2,0,-lcube/2])
	    difference(){
		rotate([0,90,90])
		cylinder(h=c,r=7/2,center=true);
		translate([7/2,0,7/2])
		rotate([0,90,90])
		cylinder(h=c+1,r=7/2,center=true);}

	    translate([lcube/2,0,-lcube/2])
	    difference(){
		rotate([0,90,90])
		cylinder(h=c,r=7/2,center=true);
		
		translate([-7/2,0,7/2])
		rotate([0,90,90])
		cylinder(h=c+1,r=7/2,center=true);}
	    
	    
	}}
}


module base(radius=a/2-modul,height=110, screws=false){
//    height=110;
    difference(){
	union(){
	    cylinder(h=height,r=radius+h,center=false);

	    cylinder(h=10,r=radius+3*h,center=false);

	    translate([0,0,-h])
	    cylinder(h=h,r=radius+3*h+.5*h,center=false);
	}
	cylinder(h=10*height,r=radius,center=true);    
    
	wheel_radius = radius+5*h;
	tyre_diameter = 4*h;
	
	translate([0,0,2*h])
	rotate_extrude(angle=360) {
	    translate([wheel_radius - tyre_diameter/2, 0])
            circle(d=tyre_diameter);}

	if (screws){
	    for(i=[0:6]){
		rotate([0,0,i*360/7])
		translate([0,radius+2.5*h,0]){
		    cylinder(h=100,r=1.5,center=true);
		    translate([0,0,0])
		    cylinder(h=h,r=4,center=true);}}}


    // for(i=[0:6]){
    // 	rotate([0,0,(i+1/4)*360/7])
    // 	translate([0,0,height/1.25]){
    // 	    rotate([0,90,0])
    // 	    cylinder(h=rtube+2*h,r=1.5,center=false);}}

}}

module lateral(width=15){
    lm3=31/2;
    l=105;
    difference(){
	union(){
	    hull(){
		rotate([0,90,0])
		cylinder(h=width,r=a/2,center=true);

		translate([0,0,a/2+15])
		cube([width,a,.01],center=true);

		translate([0,0,-b/2+db])
		rotate([0,90,0])

		minkowski(){
		    cube([43.3-5,43.3-5,width/2],center=true);
		    cylinder(h=width/2,r=2.5,center=true);}
	    }

	}

//	translate([0,-a/4,a/2+10]) cube([a/1.5,14.5,1],center=true);
	
	translate([0,a/4,a/2]) cube([a/1.5,15,6.3],center=true);
	translate([0,-a/4,a/2]) cube([a/1.5,15,6.3],center=true);

	translate([0,a/4,a/2])
	cylinder(h=50,r=4,center=true);
	translate([0,-a/4,a/2])
	cylinder(h=50,r=4,center=true);

	translate([h+.01,0,-smallgearoffset]){
	    ////////////////////////////////////////////////////////////
	    translate([0,0,-b/2+db])
	    rotate([0,90,0]){
		translate([lm3,lm3,0])
		cylinder(h=h,r=4,center=true);
		translate([-lm3,lm3,0])
		cylinder(h=h,r=4,center=true);
		translate([lm3,-lm3,0])
		cylinder(h=h,r=4,center=true);
		translate([-lm3,-lm3,0])
		cylinder(h=h,r=4,center=true);
		// Big hole

		cylinder(h=15,r=23/2+6-.8,center=true);}
	    
	    translate([0,0,-b/2+db-20])
	    cube([width,20,width],center=true);
	    ////////////////////////////////////////////////////////////
	
	translate([0,0,-b/2+db])
	rotate([0,90,0]){
	    translate([lm3,lm3,0])
	    cylinder(h=2*l,r=1.5,center=true);
	    translate([-lm3,lm3,0])
	    cylinder(h=2*l,r=1.5,center=true);
	    translate([lm3,-lm3,0])
	    cylinder(h=2*l,r=1.5,center=true);
	    translate([-lm3,-lm3,0])
	    cylinder(h=2*l,r=1.5,center=true);
	    // Big hole    
	    cylinder(h=2*l,r=23/2,center=true);}}

	rotate([0,90,0])
	cylinder(h=2*l,r=4,center=true);

	translate([gear2_bolt_sink,0,0])
	rotate([0,90,0])
	cylinder(r=.5*gear2_bolt_hex_d /cos(180/6),h=gear2_bolt_sink,$fn=6);



}

}

module arm1(height=165){
    lenght=a/2+10*cos(45)+10+1.6+7.5;
    echo(lenght);

    // big thread
    // difference(){
    // 	translate([0,0,.01/2])
    // 	cylinder(r=a/2-modul,h=30-.01,center=false);
    // 	ScrewThread(a-modul-2*h+tol, height=30, pitch=0, tooth_angle=45, tolerance=0.4, tip_height=0, tooth_height=0, tip_min_fract=0);}

    difference(){
	intersection(){
	    union(){

		hull(){
		    cube([2*lenght,a,3*h],center=true);
		    cylinder(h=3*h,r=a/2,center=true);}
		
		translate([0,0,h+h/2])
		//		base(radius=a/2-modul-h,height=height,screws=false);
		base(radius=25.4,height=height,screws=false);
	    }

	    cube([2*lenght,a,1000],center=true);}

	//screws
	translate([lenght-7.5,a/4,0])
	cylinder(h=30,r=4,center=true);
	translate([lenght-7.5,-a/4,0])
	cylinder(h=50,r=4,center=true);

	translate([-(lenght-7.5),a/4,0])
	cylinder(h=30,r=4,center=true);
	translate([-(lenght-7.5),-a/4,0])
	cylinder(h=50,r=4,center=true);


	cylinder(h=100,r=25.4-h,center=true);
	
//	translate([0,0,h])cylinder(h=3*h,r=a/2-modul-h,center=true);


}

// translate([0,-a/2-5,height-a/2.5])
// rotate([90,0,0])
// top_gear();
}



//rotate([90,0,0])



//http://diyprojects.eu/how-run-two-cnc-shields-cloned-on-one-arduino/