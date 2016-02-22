#include "block.h"
#include <stdio.h>

namespace myOctree {

int pad = 2;

//parametrized constructor with initialization fields
Field::Field( int N_x, int N_y, int N_z ) : Nx(N_x), Ny(N_y), Nz(N_z) {

        N = Nx*Ny*Nz;
        val = new double** [Nx];
        for(int i=0;i<Nx;i++) {
                val[i] = new double* [Ny];
                for(int j=0;j<Ny;j++) {
                        val[i][j] = new double [Nz];
                }
        }
}

//default constructor
Field::Field() {

        Nx = 0;
        Ny = 0;
        Nz = 0;
        N = Nx*Ny*Nz;
        val = new double** [Nx];
        for(int i=0;i<Nx;i++) {
                val[i] = new double* [Ny];
                for(int j=0;j<Ny;j++) {
                        val[i][j] = new double [Nz];
                }
        }
}

//Copy constructor
Field::Field(const Field &obj) {


        Nx = obj.Nx;
        Ny = obj.Ny;
        Nz = obj.Nz;
        N = obj.N;
        memcpy(val,obj.val,sizeof(double**)*Nx);
        for(int i=0;i<Nx;i++) {
                memcpy(val[i],obj.val[i],sizeof(double*)*Ny);
                for(int j=0;j<Ny;j++) {
                        memcpy(val[i][j],obj.val[i][j],sizeof(double)*Nz);
                }
        }
}

//Destructor
 Field::~Field() {

//	for (int i = 0; i < Nx; ++i) {
//	        for (int j = 0; j < Ny; ++j)
//	        delete [] val[i][j];
//	
//	        delete [] val[i];
//	        }
//	
//	delete [] val;
}

//member function
void Field::set_field(double value) {

	for(int i=0;i<this->Nx;i++) {
		for(int j=0;j<this->Ny;j++) {
			for(int k=0;k<this->Nz;k++) {
				this->val[i][j][k] = value;	 
			}
		}
	}
}

//parametrized constructor with initialization fields
VecField::VecField( int N_x, int N_y, int N_z ) : Nx(N_x), Ny(N_y), Nz(N_z) {

        N = Nx*Ny*Nz;
        x = new double** [Nx];
        y = new double** [Nx];
        z = new double** [Nx];
        for(int i=0;i<Nx;i++) {
                x[i] = new double* [Ny];
                y[i] = new double* [Ny];
                z[i] = new double* [Ny];
                for(int j=0;j<Ny;j++) {
                        x[i][j] = new double [Nz];
                        y[i][j] = new double [Nz];
                        z[i][j] = new double [Nz];
                }
        }
}

//default constructor
VecField::VecField() {

        Nx = 0;
        Ny = 0;
        Nz = 0;
        N = Nx*Ny*Nz;
        x = new double** [Nx];
        y = new double** [Nx];
        z = new double** [Nx];
        for(int i=0;i<Nx;i++) {
                x[i] = new double* [Ny];
                y[i] = new double* [Ny];
                z[i] = new double* [Ny];
                for(int j=0;j<Ny;j++) {
                        x[i][j] = new double [Nz];
                        y[i][j] = new double [Nz];
                        z[i][j] = new double [Nz];
                }
        }
}

//Copy constructor
VecField::VecField(const VecField &obj) {


        Nx = obj.Nx;
        Ny = obj.Ny;
        Nz = obj.Nz;
        N = obj.N;
        memcpy(x,obj.x,sizeof(double**)*Nx);
        memcpy(y,obj.y,sizeof(double**)*Nx);
        memcpy(z,obj.z,sizeof(double**)*Nx);
        for(int i=0;i<Nx;i++) {
                memcpy(x[i],obj.x[i],sizeof(double*)*Ny);
                memcpy(y[i],obj.y[i],sizeof(double*)*Ny);
                memcpy(z[i],obj.z[i],sizeof(double*)*Ny);
                for(int j=0;j<Ny;j++) {
                        memcpy(x[i][j],obj.x[i][j],sizeof(double)*Nz);
                        memcpy(y[i][j],obj.y[i][j],sizeof(double)*Nz);
                        memcpy(z[i][j],obj.z[i][j],sizeof(double)*Nz);
                }
        }
}

//Destructor
 VecField::~VecField() {

//	for (int i = 0; i < Nx; ++i) {
//	        for (int j = 0; j < Ny; ++j)
//	        delete [] val[i][j];
//	
//	        delete [] val[i];
//	        }
//	
//	delete [] val;
}

//member function
void VecField::set_field(double value) {

	for(int i=0;i<this->Nx;i++) {
		for(int j=0;j<this->Ny;j++) {
			for(int k=0;k<this->Nz;k++) {
				this->x[i][j][k] = value;	 
				this->y[i][j][k] = value;	 
				this->z[i][j][k] = value;	 
			}
		}
	}
}

//parametrized constructor with initialization fields
Block::Block( double x1, double x2, double y1, double y2, double z1, double z2 ) : x_min(x1), x_max(x2), y_min(y1), y_max(y2), z_min(z1), z_max(z2) {

        dx = ( x_max - x_min ) / iNx;
        dy = ( y_max - y_min ) / iNy;
        dz = ( z_max - z_min ) / iNz;

        //printf("dx=%g, dy=%g, dz=%g \n", dx, dy, dz);

        x_centre = (x_min + x_max ) / 2.0;
        y_centre = (y_min + y_max ) / 2.0;
        z_centre = (z_min + z_max ) / 2.0;

        //dynamical allocation of the objects
        mesh = new VecField;
        VecField mesh_field(iNx+2*pad,iNy+2*pad,iNz+2*pad);
        *mesh = mesh_field;
        
	field = new Field;
        Field field_field(iNx+2*pad,iNy+2*pad,iNz+2*pad);
        *field = field_field;
       
	/*no ghost cells for this*/ 
	gradient = new VecField;
        VecField gradient_field(iNx+2*pad,iNy+2*pad,iNz+2*pad);
        *gradient = gradient_field;

	//storing cell centre locations in mesh vector field
	for(int i=0;i<mesh->Nx;i++) {
		for(int j=0;j<mesh->Ny;j++) {
			for(int k=0;k<mesh->Nz;k++) {
				mesh->x[i][j][k] = x_min + dx * (i + 0.5);	 
				mesh->y[i][j][k] = y_min + dy * (j + 0.5);	 
				mesh->z[i][j][k] = z_min + dz * (k + 0.5);	 
			}
		}
	}
	

        //printf("N=%d\n",mesh->N);

}

//default constructor
Block::Block() {

        mesh = new VecField;
        VecField mesh_field(iNx+2*pad,iNy+2*pad,iNz+2*pad);
        *mesh = mesh_field;
	
	field = new Field;
        Field field_field(iNx+2*pad,iNy+2*pad,iNz+2*pad);
        *field = field_field;
	
	/*no ghost cells for this*/ 
	gradient = new VecField;
        VecField gradient_field(iNx+2*pad,iNy+2*pad,iNz+2*pad);
        *gradient = gradient_field;
}

//Copy constructor
Block::Block(const Block &obj) {

        x_centre = obj.x_centre;
        y_centre = obj.y_centre;
        z_centre = obj.z_centre;
        x_min = obj.x_min;
        y_min = obj.y_min;
        x_min = obj.x_min;
        x_max = obj.x_max;
        y_max = obj.y_max;
        z_max = obj.z_max;
        dx = obj.dx;
        dy = obj.dy;
        dz = obj.dx;
        iNx = obj.iNx;
        iNy = obj.iNy;
        iNz = obj.iNz;
        mesh = obj.mesh;
        field = obj.field;
	gradient = obj.gradient;
	max_gradient = obj.max_gradient;
}

//Destructor
Block::~Block() {

//        delete mesh;

}

//member function
void Block::calculate_grid_size() {

        this->dx = ( this->x_max - this->x_min ) / this->iNx;
        this->dy = ( this->y_max - this->y_min ) / this->iNy;
        this->dz = ( this->z_max - this->z_min ) / this->iNz;
}

//member function
void Block::calculate_centre() {
        this->x_centre = (this->x_min + this->x_max ) / 2.0;
        this->y_centre = (this->y_min + this->y_max ) / 2.0;
        this->z_centre = (this->z_min + this->z_max ) / 2.0;
}


}
