/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license APACHE-2 (see license file)
 */


#include <appl/debug.hpp>

int32_t appl::getLogId() {
	static int32_t g_val = elog::registerInstance("GP-spaceShip");
	return g_val;
}
