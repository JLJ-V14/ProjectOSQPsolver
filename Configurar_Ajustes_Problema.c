#include "osqp.h"
void Configurar_Ajustes(OSQPSettings* settings) {
	osqp_set_default_settings(settings);
	// Set your specific settings
	settings->eps_abs = 1e-6; // Absolute convergence tolerance
	settings->eps_rel = 1e-6; // Relative convergence tolerance
	settings->max_iter = 500000; // Maximum number of iterations
	settings->adaptive_rho = 1; // Enable adaptive rho
	settings->adaptive_rho_interval = 60; // Update rho every 60 iterations
}
