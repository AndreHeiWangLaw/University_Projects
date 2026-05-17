
# Health & Education Facility Management System

A robust, enterprise-grade relational database application designed to streamline administrative workflows, personnel deployment, scheduling operations, and public health tracking across interconnected regional ministries, management facilities, and educational institutions. 

Built using a raw PHP architecture and an optimized relational MySQL/MariaDB backend, this system implements multi-tiered enterprise logic, secure data access patterns, and dynamic reporting to manage complex organizational entities while maintaining strict public health compliance.

---

## 🚀 Key Architectural Modules

### 1. Unified Entity Directory & Access Control (CRUD)
* **Comprehensive Personnel Registry:** Manages master profiles for citizens (`Person`) capturing demographics, contact tracking, citizenship status, and real-time validation of government-issued credentials (`MedicareCardNumber`, expiration logs).
* **Dynamic Role Polymorphism:** Restructures administrative workflows by separating entities into granular sub-types such as `Student` (with instructional tiers) and `Employee` (Teachers, Administrative Staff, Facility Presidents) based on real-time organizational needs.
* **Automated Facility Enrollment:** Provides synchronous, transaction-safe onboarding pathways where a new entry is simultaneously registered to the central system and dynamically assigned to specific local institutions.

### 2. Algorithmic Scheduling Engine
* **Shift Optimization Matrix:** Features an administrative controller (`assignSchedule.php`) allowing operators to construct conflict-free weekly timelines mapping specific employees to target facilities, dates, and non-overlapping shifts.
* **Temporal Query Engine:** Employs an AJAX-driven interface (`schedule.html`) backed by dynamic date-range query processors (`searchSchedules.php`) to generate instantaneous, granular shift rosters for personnel auditing.

### 3. Public Health Tracker & Infection Protocols
* **Vaccination Passport Ledger:** Houses clinical logging mechanisms (`vacPage.php`) tracking multi-dose inoculation events, specific manufacturer types, and administration timelines across all registered individuals.
* **Diagnostic Audit Trail:** Integrates an infection tracking matrix (`infectionPage.php`) that cross-references active health anomalies against facility logs to dynamically assess operational safety risks.

### 4. Enterprise Reporting & Analytics Dashboard
* **Organizational Hierarchy Mapping (`8.php`):** Executes complex conditional aggregations (`CASE WHEN`) to dynamically determine facility leadership (Principals vs. Corporate Presidents), extracting their profile records via synchronized subqueries while computing active staff volumes.
* **Ministry Analytics Matrix (`16.php`):** Aggregates complex relational parameters across federal portfolios, rendering city-by-city summaries of operational capacities, ministerial control, and structural metrics.

---

## 🛠️ Technology Stack & Engineering Disciplines

* **Backend Engineering:** Procedural & Structural PHP 8.x.
* **Database Management System:** MySQL / MariaDB relational engine running optimized 3rd Normal Form (3NF) structural schemas.
* **Security Architecture:** Implements parameterized queries and SQL prepared statements (`mysqli_prepare`, `bind_param`) across data modification paths to neutralize SQL Injection (SQLi) threat profiles.
* **Data Integrity & Cascades:** Implements strict foreign key constraints and transactional cleanup modules (`deleteEmployee.php`) ensuring absolute database referential integrity by safely eliminating orphan data across related scheduling and medical tables upon record termination.
* **Frontend Interfaces:** Standard-compliant HTML5 semantic components paired with asynchronous JavaScript engines to provide responsive administrative interactions.

---

## 📁 Core Directory Structure

```text
├── index.php                 # Core administrative landing page & operational control center
├── facilityPage.php          # Management interface for organizational installations and analytics
├── personPage.php            # Master directory wrapper for registering and looking up individuals
├── studentPage.php           # Specialized portal managing student lifecycles and enrollment levels
├── employeePage.php          # Professional workforce profiling, management, and tracking interface
├── schedule.html             # Dynamic UI for shift configurations and personnel roster views
├── searchSchedules.php       # Temporal lookup logic generating employee shift breakdowns
├── assignSchedule.php        # Transaction controller mapping employees to active facility shifts
├── infectionPage.php         # Diagnostic tracking engine for inputting healthcare alerts
├── vacPage.php               # Immunization ledger monitoring corporate health safety metrics
├── addPerson.php             # Unified entity instantiation handler enforcing strict field validation
├── deleteEmployee.php        # Cascade-safe erasure module maintaining system referential integrity
└── 16.php                    # Higher-level administrative business intelligence report for Ministries
