include <gears.scad>

$fn=100;

thk=4;
nop=0.01;
screw=5.2;
bear=12; //bearing horizontal
bear_c=0.2; //bearing clearance
arm_c=0.2; //arm clearance
insert=3.8;
stteper_hole=12;

// HELPERS =====================================//

module rounded_square(s, r) {
    k = s/2 - r;
    
	hull() {
		translate([k, k, 0])
        circle(r);
		translate([k, -k, 0])
        circle(r);
		translate([-k, -k, 0])
        circle(r);
		translate([-k, k, 0])
        circle(r);
	}
}

module half_cylinder(h, d) {
    intersection() {
        cylinder(h=h, d=d, center=true);
        translate([0, d/4, 0])
        cube([d, d/2, h], center=true);
    }
}

module axis(h, d, c) {
   intersection() {
        cylinder(h=h, d=d);
        cube([c, d, 2*h], center=true);
    }
} 

// STEPPER =====================================//

module stepper() {
    translate([0, 8, -19/2])
    scale(10)
    import("stepper.stl", convexity=3);
}

module steppers(dx) {
    color("grey")
    for(i = [-1, 1]) {
        translate([i*dx, 0, 0])
        rotate([90, 0, -i*90])
        stepper();
    }
}

// DICE ========================================//

module dice(in, out, d, f, bore) {
    difference() {
        intersection() {
            cube(out, center=true);
            sphere(d=d);
        }
        
        rotate([90, 0, 0])
        linear_extrude(out+nop, center=true)
        rounded_square(in, f);
        
        cylinder(out+nop, d=bore,
        center=true);
        
        rotate([0, 90, 0])
        cylinder(out+nop, d=bore,
        center=true);
    }
}

// ARM =========================================//

module arm(size, gear_x) {
    dx = (gear_x + thk/2 + arm_c);
    side_h = sqrt(pow(gear_x+10, 2) + pow(10/2, 2))+2;
   
    for(i = [-1, 1]) {
        translate([i*dx, 0, 0]) {
            difference() {
                union() {
                    rotate([-90, 0, 90])
                    half_cylinder(thk, size);
                    translate([0, 0, side_h/2])
                    cube([thk, size, side_h+nop],
                        center=true);
                }
                
                for(j = [-1, 1])
                translate([0, j*35/2, 8])
                rotate([0, 90, 0])
                cylinder(h=thk+nop, d=insert, center=true);
                
                rotate([0, 90, 0])
                cylinder(h=thk+nop, d=stteper_hole, 
                    center=true);
            }
            
        }
    }
    
    translate([0, 0, thk/2+side_h])
    difference() {
        cube([2*dx+thk, size, thk], center=true); 
        cylinder(h=thk+nop, d=screw, center=true);
    }
}

// GEARS =======================================//

module differential(){
    modul = 1.25;
    teeth = 33;
    tooth_width = 10;
    pressure_angle = 20;
    helix_angle = 10;
    clearance = 0.05;

    
    // Math from bevel_herringbone_gear_pair()
    r = modul*teeth/2;
    rg = r/sin(45);
    c = modul / 6;   
    df = pi*rg/2 - 2 * (modul + c);    
    rf = df / 2;            
    delta_f = rf/(pi*rg) * 180; 
    height_f = rg*cos(delta_f);               
    height_k = (rg-tooth_width)/cos(45);      
    rk = (rg-tooth_width)/sin(45);   
    height_fk = rk*height_k/(height_k*tan(delta_f)+rk); 
    height  = height_f-height_fk;
    ro = rg + 2*height/tan(45);
    
    rot = 180*(1-clearance)/teeth*is_even(teeth);
    
    dice_out = 2*height_fk - 0.5;
    dice_in = dice_out-2*thk;
    
    module gear(comp) {
        tmp = (comp) ? -1 : 1;
        
        union() {
            bevel_herringbone_gear(
                modul=modul,
                tooth_number=teeth,
                partial_cone_angle=45,
                tooth_width=tooth_width,
                bore=screw,
                pressure_angle=pressure_angle,
                helix_angle=tmp*helix_angle);
            
             cylinder(h=height_f-dice_in/2+bear_c, d=bear);
        }
    }
    
    for(i = [-1, 1])
    rotate([0,0,rot]) 
    translate([0, 0, i*height_f])
    rotate([0, (i+1)*90, 0])
    difference() {
        gear(false);
        translate([0, 0, -nop])
        cylinder(h=height_f-dice_in/2+bear_c+2*nop, d=screw);
    }
     
    for(i = [-1, 1])
    translate([i*height_f, 0, 0])
    rotate([0, -i*90, 0]) 
    difference() {
        gear(true);   
        axis(height_f, 5, 3);
    }
    
    dice(out=dice_out, in=dice_out-2*thk,
    d=1.2*dice_out, f=2.5, bore=bear+2*bear_c);
    
    arm(size=ro, gear_x=height_f);
    
    if($preview) {
        steppers(dx=height_f+thk);
    }
    
    echo("dice_in = ", dice_in);
}

differential();



