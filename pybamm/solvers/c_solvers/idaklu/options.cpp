#include "options.hpp"
#include <stdexcept>

 
using namespace std::string_literals;

Options::Options(py::dict options)
    : print_stats(options["print_stats"].cast<bool>()),
      jacobian(options["jacobian"].cast<std::string>()),
      preconditioner(options["preconditioner"].cast<std::string>()),
      linsol_max_iterations(options["linsol_max_iterations"].cast<int>()),
      linear_solver(options["linear_solver"].cast<std::string>()),
      precon_half_bandwidth(options["precon_half_bandwidth"].cast<int>()),
      precon_half_bandwidth_keep(options["precon_half_bandwidth_keep"].cast<int>())
{

  using_sparse_matrix = true;
  if (jacobian == "sparse")
  {
  }
  else if (jacobian == "dense" || jacobian == "none")
  {
    using_sparse_matrix = false;
  }
  else if (jacobian == "matrix-free")
  {
  }
  else
  {
    throw std::domain_error(
      "Unknown jacobian type \""s + jacobian + 
      "\". Should be one of \"sparse\", \"dense\", \"matrix-free\" or \"none\"."s
    );
  }

  using_iterative_solver = false;
  if (linear_solver == "SUNLinSol_Dense" && jacobian == "dense")
  {
  }
  else if (linear_solver == "SUNLinSol_LapackDense" && jacobian == "dense")
  {
  }
  else if (linear_solver == "SUNLinSol_KLU" && jacobian == "sparse")
  {
  }
  else if ((linear_solver == "SUNLinSol_SPBCGS" ||
            linear_solver == "SUNLinSol_SPFGMR" ||
            linear_solver == "SUNLinSol_SPGMR" ||
            linear_solver == "SUNLinSol_SPTFQMR") &&
           (jacobian == "sparse" || jacobian == "matrix-free"))
  {
    using_iterative_solver = true;
  }
  else if (jacobian == "sparse")
  {
    throw std::domain_error(
      "Unknown linear solver or incompatible options. For a sparse jacobian "
      "please use the SUNLinSol_KLU linear solver"
    );
  }
  else if (jacobian == "matrix-free")
  {
    throw std::domain_error(
        "Unknown linear solver or incompatible options. For a matrix-free jacobian "
        "please use one of the iterative linear solvers: \"SUNLinSol_SPBCGS\", "
        "\"SUNLinSol_SPFGMR\", \"SUNLinSol_SPGMR\", or \"SUNLinSol_SPTFQMR\"."
      );
  }
  else
  {
    throw std::domain_error(
      "Unknown linear solver \""s + linear_solver + 
      "\", use one of \"SUNLinSol_KLU\", \"SUNLinSol_Dense\", "
      "\"SUNLinSol_LapackDense\", \"SUNLinSol_SPBCGS\", \"SUNLinSol_SPFGMR\", "
      "\"SUNLinSol_SPGMR\", or \"SUNLinSol_SPTFQMR\""
    );
  }

  if (using_iterative_solver)
  {
    if (preconditioner != "none" && preconditioner != "BBDP")
    {
      throw std::domain_error(
        "Unknown preconditioner \""s + preconditioner + 
        "\", use one of \"BBDP\" or \"none\""s
      );
    }
  }
  else
  {
    preconditioner = "none";
  }
}
