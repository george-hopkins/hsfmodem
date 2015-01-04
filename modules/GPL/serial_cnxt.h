/*
 * Copyright (c) 2003-2004 Linuxant inc.
 * 
 * 1.  General Public License. This program is free software, and may
 * be redistributed or modified subject to the terms of the GNU General
 * Public License (version 2) or the GNU Lesser General Public License,
 * or (at your option) any later versions ("Open Source" code). You may
 * obtain a copy of the GNU General Public License at
 * http://www.fsf.org/copyleft/gpl.html and a copy of the GNU Lesser
 * General Public License at http://www.fsf.org/copyleft/less.html,
 * or you may alternatively write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA.
 * 
 * 2.   Disclaimer of Warranties. CONEXANT AND OTHER CONTRIBUTORS MAKE NO
 * REPRESENTATION ABOUT THE SUITABILITY OF THIS SOFTWARE FOR ANY PURPOSE.
 * IT IS PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTIES OF ANY KIND.
 * CONEXANT AND OTHER CONTRIBUTORS DISCLAIMS ALL WARRANTIES WITH REGARD TO
 * THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE, GOOD TITLE AND AGAINST INFRINGEMENT.
 * 
 * This software has not been formally tested, and there is no guarantee that
 * it is free of errors including, but not limited to, bugs, defects,
 * interrupted operation, or unexpected results. Any use of this software is
 * at user's own risk.
 * 
 * 3.   No Liability.
 * 
 * (a) Conexant or contributors shall not be responsible for any loss or
 * damage to user, or any third parties for any reason whatsoever, and
 * CONEXANT OR CONTRIBUTORS SHALL NOT BE LIABLE FOR ANY ACTUAL, DIRECT,
 * INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL, OR CONSEQUENTIAL
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED, WHETHER IN CONTRACT, STRICT OR OTHER LEGAL THEORY OF
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
 * WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 * 
 * (b) User agrees to hold Conexant and contributors harmless from any
 * liability, loss, cost, damage or expense, including attorney's fees,
 * as a result of any claims which may be made by any person, including
 * but not limited to User, its agents and employees, its customers, or
 * any third parties that arise out of or result from the manufacture,
 * delivery, actual or alleged ownership, performance, use, operation
 * or possession of the software furnished hereunder, whether such claims
 * are based on negligence, breach of contract, absolute liability or any
 * other legal theory.
 * 
 * 4.   Notices. User hereby agrees not to remove, alter or destroy any
 * copyright, trademark, credits, other proprietary notices or confidential
 * legends placed upon, contained within or associated with the Software,
 * and shall include all such unaltered copyright, trademark, credits,
 * other proprietary notices or confidential legends on or in every copy of
 * the Software.
 * 
 */
#ifndef __SERIALCNXT_H
#define __SERIALCNXT_H

#include "osresour_ex.h"

#ifdef __cplusplus
extern "C"
{
#endif

int cnxt_serial_add(POS_DEVNODE devnode, unsigned int iobase, void *membase, unsigned int irq, struct module *owner);
int cnxt_serial_remove(POS_DEVNODE devnode);

#ifdef __cplusplus
}
#endif

#endif /* __SERIALCNXT_H */
