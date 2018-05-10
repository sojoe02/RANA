//--begin_license--
//
//Copyright 	2013 	Søren Vissing Jørgensen.
//			2014	Søren Vissing Jørgensen, Center for Bio-Robotics, SDU, MMMI.
//
//This file is part of RANA.
//
//RANA is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//RANA is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with RANA.  If not, see <http://www.gnu.org/licenses/>.
//
//--end_license--

#include <QApplication>
#include <stdio.h>
#include <string>

#include "src/ID.h"
#include "src/cli.h"
#include "src/mainwindow.h"
#include "src/api/phys.h"
#include "src/api/gridmovement.h"
#include "src/output.h"

#include "src/parser.h"

#include "src/api/tcpserver.h"
#include "src/api/tcpclient.h"

int ID::aID = 0;
unsigned long long ID::eID = 0;
unsigned long long ID::tmu = 0;
unsigned long long ID::nID = 0;

/*
#include "bayesopt/bayesopt.hpp"
#include <boost/numeric/ublas/assignment.hpp>
#include <ctime>

double testFunction(unsigned int n, const double *x,double *gradient, void *func_data)
{
  double f = 10.;
  for (unsigned int i = 0; i < n; ++i)
    {
      f += (x[i] - .65) * (x[i] - .65);
    }
  return f;
}

class ExampleQuadratic: public bayesopt::ContinuousModel
{
    public:
        ExampleQuadratic(size_t dim,bayesopt::Parameters param):
        ContinuousModel(dim,param) {}

    double evaluateSample( const vectord &Xi ){
        double x[100];
        for (size_t i = 0; i < Xi.size(); ++i){
            x[i] = Xi(i);
        }
        return testFunction(Xi.size(),x,nullptr,nullptr);
    }

    bool checkReachability( const vectord &query ){ return true; }
};

int main(int argc, char **argv)
{
    int n = 5;                   // Number of dimensions
    clock_t start, end;
    double diff;
    // Common configuration
    // See parameters.h for the available options.
    // Some parameters did not need to be changed for default, but we have done it for
    // illustrative purpose.
    bayesopt::Parameters par = initialize_parameters_to_default();
    par.kernel.name = "kSum(kSEISO,kConst)";
    par.kernel.hp_mean <<= 1.0;
    par.kernel.hp_std <<= 1.0;

    par.mean.name = "mConst";
    par.mean.coef_mean <<= 1.0;
    par.mean.coef_std <<= 1.0;


    par.surr_name = "sStudentTProcessJef";
    par.noise = 1e-10;

    par.sc_type = SC_MAP;
    par.l_type = L_EMPIRICAL;

    par.n_iterations = 100;    // Number of iterations
    par.random_seed = 0;
    par.n_init_samples = 15;
    par.n_iter_relearn = 0;

    ExampleQuadratic opt(n,par);
    vectord result(n);

    start = clock();
    opt.optimize(result);
    end = clock();
    diff = double(end-start) / double(CLOCKS_PER_SEC);

    // Results
    std::cout << "Final result C++: " << result << std::endl;
    std::cout << "Elapsed time in C++: " << diff << " seconds" << std::endl;

}
*/

/*
int main(int argc, char **argv)
{


        parser *p = parser::getInstance();
        p->parseInputArgs(argc, argv);

        //srand(time(0));
        Phys::seedMersenne();
        Output::DelayValue = 0;
        Output::LegacyMode.store(false);
        GridMovement::initGrid(1);

        qDebug() << Phys::getMersenneInteger(1, RAND_MAX) << Phys::getMersenneInteger(1, RAND_MAX) << Phys::getMersenneFloat(1, RAND_MAX) <<Phys::getMersenneInteger(1, RAND_MAX) ;

        QApplication a(argc, argv);

        if(p->startProgram())
        {
            if(p->startGui()){
                std::cout << "Start with gui" << std::endl;
                MainWindow *w = new MainWindow();
                Output::Inst()->setMainWindow(w);
                w->show();
            }else{
                std::cout << "Start without gui - " << p->getFile() << std::endl;
                Cli *c = new Cli(p->getFile());
            }
        }

        return a.exec();

}
*/

#include "src/bopthook.h"

int main()
{


    bayesopt::Parameters param;
    param.l_type = L_MCMC;
    //param.n_iterations = 10;

    int n = 10;                   // Number of dimensions
    bopthook optimizer(n, param);
    vectord result(n);

    optimizer.optimize(result);


}

































