<?php
/**
 * @file
 * As simple as possible migrations.
 *
 * You can run these migration with:
 * - drush mi CommerceExampleProduct
 *
 * To roll back migrations:
 * - drush migrate-rollback CommerceExampleProductDisplay
 *
 * To see the status of all migrations and what migrations are available:
 * - drush ms
 *
 * Please make sure you don't get one of the products into the cart while
 * you're rolling back, as this makes no end of messiness. Products in the
 * cart are (not supposed to be able) to be deleted.
 */

/**
 * Implements hook_migrate_api().
 */
function commerce_migrate_example_migrate_api() {
  $info = array(
    'api' => 2,
    'groups' => array(
      'commerce_example' => array(
        'title' => t('Commerce Example Imports'),
      ),
    ),
  );

  $info['migrations']['CommerceExampleProduct'] = array(
    'class_name' => 'CommerceExampleProductMigration',
    'group_name' => 'commerce_example',
    'source_file' => 'products.csv',
  );

  $info['migrations']['CommerceExampleProductDisplay'] = array(
    'class_name' => 'CommerceExampleProductDisplayMigration',
    'group_name' => 'commerce_example',
    'source_file' => 'products.csv',
    'dependencies' => array(
      'CommerceExampleProduct',
    ),
  );

  $info['migrations']['CommerceExampleProductLineItem'] = array(
    'class_name' => 'CommerceExampleProductLineItemMigration',
    'group_name' => 'commerce_example',
    'source_file' => 'products.csv',
    'dependencies' => array(
      'CommerceExampleProduct',
    ),
  );

  $info['migrations']['CommerceExampleOrder'] = array(
    'class_name' => 'CommerceExampleOrderMigration',
    'group_name' => 'commerce_example',
    'source_file' => 'orders.csv',
    'dependencies' => array(
      'CommerceExampleProductLineItem',
    ),
  );

  return $info;
}

/**
 * A simple base class that contains things common to all
 * CommerceMigrateExample classes.
 */
abstract class CommerceMigrateExampleMigration extends Migration {

  /**
   * Module path.
   *
   * @var string
   */
  protected $modulePath = '';

  /**
   * {@inheritdoc}
   */
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    if (empty($arguments['source_file'])) {
      throw new Exception(t('Name of CSV file with source data has not been set.'));
    }

    $this->modulePath = drupal_get_path('module', 'commerce_migrate_example');
    $csv_file = "$this->modulePath/files/{$arguments['source_file']}";

    if (!file_exists($csv_file)) {
      throw new Exception(t('The "@csv_file" CSV file does not exists.', array(
        '@csv_file' => $csv_file,
      )));
    }

    // Create a MigrateSource object, which manages retrieving the input data.
    $this->source = new MigrateSourceCSV($csv_file, $this->csvColumns());
  }

  /**
   * Create a map object for tracking the relationships between source rows.
   *
   * @param array $destination_schema
   *   Values of any MigrateDestination::getKeySchema().
   */
  protected function buildMap($destination_schema) {
    // Create a map object for tracking the relationships between source rows.
    $columns = $this->csvColumns();
    // The first column in CSV - the primary index.
    list($key, $name) = $columns[0];

    $this->map = new MigrateSQLMap(
      $this->machineName,
      array(
        // The 'SKU' field in the CSV file is the key.
        $key => array(
          'type' => 'varchar',
          'length' => 24,
          'not null' => TRUE,
          'description' => $name,
        ),
      ),
      $destination_schema
    );
  }

  /**
   * Provide the names of the incoming CSV file columns.
   *
   * @return array[]
   *   An array with column name and title.
   */
  abstract protected function csvColumns();

}

/**
 * Class CommerceMigrateExampleProductsCsv.
 *
 * Represents "products.csv".
 */
abstract class CommerceMigrateExampleProductsCsv extends CommerceMigrateExampleMigration {

  /**
   * {@inheritdoc}
   */
  protected function csvColumns() {
    $columns = array();
    $columns[] = array('sku', 'SKU');
    $columns[] = array('title', 'Title');
    $columns[] = array('price', 'Price');
    $columns[] = array('currency_code', 'Currency code');
    $columns[] = array('image', 'Image');
    $columns[] = array('description', 'Description');

    return $columns;
  }

}

/**
 * Class CommerceMigrateExampleOrdersCsv.
 *
 * Represents "orders.csv".
 */
abstract class CommerceMigrateExampleOrdersCsv extends CommerceMigrateExampleMigration {

  /**
   * {@inheritdoc}
   */
  protected function csvColumns() {
    $columns = array();
    $columns[] = array('number', 'Order number');

    return $columns;
  }

}
