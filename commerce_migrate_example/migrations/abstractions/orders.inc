<?php
/**
 * @file
 * Represents "orders.csv".
 */

/**
 * Class CommerceMigrateExampleOrdersCsv.
 */
abstract class CommerceMigrateExampleOrdersCsv extends \CommerceMigrateExampleMigration {

  /**
   * {@inheritdoc}
   */
  protected function csvColumns() {
    return array(
      'order_number' => 'Number',
      'status' => 'Number',
      'mail' => 'Number',
      'hostname' => 'Hostname',
    );
  }

}
